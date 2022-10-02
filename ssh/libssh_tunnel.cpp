#include "libssh_tunnel.h"

#include "db/exception.h"
#include "app/log.h"
#include "helpers/logger.h"
#include "libssh_connection.h"

#include <libssh/libssh.h>

#include <QDebug>

#include <memory>

namespace meow::ssh
{

LibSSHTunnel::LibSSHTunnel()
    : _session(std::make_shared<LibSSH>())
{

}

LibSSHTunnel::~LibSSHTunnel()
{
    for(const auto& connection: _connections) {
        connection.second.first->close();
    }
    _session.reset();
    _stopThread = true;
    if (_thread.joinable()) {
        _thread.join();
    }
}

bool LibSSHTunnel::connect(const meow::db::ConnectionParameters& params)
{
    _params = params;

    int verbosity = SSH_LOG_WARNING;

    const QByteArray hostBytes = _params.sshTunnel().host().toLatin1();
    const QByteArray userBytes = _params.sshTunnel().user().toLatin1();
    const char* host = hostBytes.constData();
    const char* user = userBytes.constData();
    int port = _params.sshTunnel().port();

    _session->setOption(SSH_OPTIONS_HOST, host);
    _session->setOption(SSH_OPTIONS_USER, user);
    _session->setOption(SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    _session->setOption(SSH_OPTIONS_PORT, &port);

    if (_session->connect() != SSH_OK) {
        failWithError(errorString());
        return false;
    }

    if (!verifyHost()) {
        failWithError("Unable to verify SSH host");
        return false;
    }

    if (!_params.sshTunnel().password().isEmpty()) {
        if (!authWithPassword()) {
            failWithError(errorString());
            return false;
        }
        qDebug() << "Auth with password succeded";
    } else {
        if (!authWithPublicKey()) {
            failWithError(errorString());
            return false;
        }
    }


    _socket = std::make_unique<meow::Socket>(shared_from_this());
    _socket->listen("127.0.0.1", 0);
    _params.sshTunnel().setLocalPort(_socket->port());

    std::unique_lock<std::mutex> lk(_threadMutex);
    if (_threadRunning) {
        throw std::runtime_error("Thread already running");
    }
    _thread = std::thread(&LibSSHTunnel::threadFunc, this);
    _threadWait.wait(lk, [&]{
        return _threadRunning;
    });
    return true;
}

bool LibSSHTunnel::supportsPassword() const
{
    return true;
}

SSHTunnelParameters LibSSHTunnel::params() const
{
    return _params.sshTunnel();
}

QString LibSSHTunnel::errorString() const
{
    return { _session->getError() };
}

void LibSSHTunnel::failWithError(const QString& error)
{
    meowLogC(Log::Category::Error)
        << "SSH Tunnel failed: "
        << error;
    QString errorStr = QObject::tr("SSH Tunnel failed: ") + error;

    throw db::Exception(errorStr);
}

bool LibSSHTunnel::verifyHost()
{
    auto state = _session->isKnownServer();

    if (state != SSH_KNOWN_HOSTS_OK) {
        // TODO - display prompt to user
        failWithError(
            "Host is not cached - TODO!! - For now, failing for safety");
        return false;
    }

    return true;
}

bool LibSSHTunnel::authWithPassword()
{
    QByteArray passwordBytes = _params.sshTunnel().password().toUtf8();
    const char* password = passwordBytes.constData();
    return _session->userAuthPassword(password) == SSH_AUTH_SUCCESS;
}

bool LibSSHTunnel::authWithPublicKey()
{
    return _session->userAuthPublicKeyAuto(nullptr) == SSH_AUTH_SUCCESS;
}

void LibSSHTunnel::disconnect()
{
    for(const auto& connection : _connections) {
        connection.second.first->close();
    }
    _connections.clear();
}

std::shared_ptr<IConnectionReceiver> LibSSHTunnel::onNewConnection(
        const std::shared_ptr<Connection>& connection)
{
    std::unique_ptr<LibSSHChannel> channel;
    try {
        channel = _session->newChannel();
    }
    catch (const std::runtime_error& err) {
        return {};
    }

    auto sshConnection = std::make_shared<LibSSHConnection>(
                _params,
                _session,
                std::move(channel),
                connection,
                shared_from_this());

    size_t connectionID = connection->connectionID();
    _connections.insert({ connectionID, std::make_pair(connection, sshConnection) });
    sshConnection->startTunnel();
    return sshConnection;
}

void LibSSHTunnel::onConnectionClosed(size_t socketID)
{
    auto it = _connections.find(socketID);
    if (it != _connections.end()) {
        _connections.erase(it);
    }
}

void LibSSHTunnel::threadFunc()
{
    {
        std::unique_lock<std::mutex> lk(_threadMutex);
        _threadRunning = true;
        _threadWait.notify_all();
    }
    while (!_stopThread) {
        _socket->pollForMs(50);
    }
}
}

