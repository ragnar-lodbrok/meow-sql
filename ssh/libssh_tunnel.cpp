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
libssh_tunnel::libssh_tunnel()
    : _session(std::make_shared<libssh>())
{

}
bool libssh_tunnel::connect(const meow::db::ConnectionParameters& params)
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


    _socket = std::make_unique<meow::socket>(shared_from_this());
    _socket->listen("127.0.0.1", 0);
    _params.sshTunnel().setLocalPort(_socket->getPort());

    std::unique_lock<std::mutex> lk(_threadMutex);
    if (_threadRunning)
    {
        throw std::runtime_error("Thread already running");
    }
    _thread = std::thread(&libssh_tunnel::threadFunc, this);
    _threadWait.wait(lk, [&]{
        return _threadRunning;
    });
    return true;
}

bool libssh_tunnel::supportsPassword() const
{
    return true;
}

SSHTunnelParameters meow::ssh::libssh_tunnel::params() const
{
    return _params.sshTunnel();
}

libssh_tunnel::~libssh_tunnel()
{
    for(const auto& connection: _connections)
    {
        connection.second.first->close();
    }
    _session.reset();
    _stopThread = true;
    if (_thread.joinable())
    {
        _thread.join();
    }
}

QString libssh_tunnel::errorString() const
{
    return {_session->getError()};
}

void libssh_tunnel::failWithError(const QString& error)
{
    meowLogC(Log::Category::Error)
        << "SSH Tunnel failed: "
        << error;
    QString errorStr = QObject::tr("SSH Tunnel failed: ") + error;

    throw db::Exception(errorStr);
}

bool libssh_tunnel::verifyHost()
{
    auto state = _session->isKnownServer();

    if (state != SSH_KNOWN_HOSTS_OK)
    {
        // TODO - display prompt to user
        failWithError("Host is not cached - TODO!! - For now, failing for safety");
        return false;
    }

    return true;
}

bool libssh_tunnel::authWithPassword()
{
    QByteArray passwordBytes = _params.sshTunnel().password().toUtf8();
    const char* password = passwordBytes.constData();
    return _session->userAuthPassword(password) == SSH_AUTH_SUCCESS;
}

bool libssh_tunnel::authWithPublicKey()
{
    return _session->userAuthPublicKeyAuto(nullptr) == SSH_AUTH_SUCCESS;
}

void libssh_tunnel::disconnect()
{
    for(const auto& connection: _connections)
    {
        connection.second.first->close();
    }
    _connections.clear();
}

std::shared_ptr<IConnectionReceiver> libssh_tunnel::onNewConnection(const std::shared_ptr<connection>& connection)
{
    std::unique_ptr<libssh_channel> channel;
    try {
        channel = _session->newChannel();
    }
    catch (const std::runtime_error& err) {
        return {};
    }

    auto sshConnection = std::make_shared<libssh_connection>(_params, _session, std::move(channel), connection, shared_from_this());
    size_t connectionID = connection->connectionID();
    _connections.insert({ connectionID, std::make_pair(connection, sshConnection) });
    sshConnection->startTunnel();
    return sshConnection;
}

void libssh_tunnel::onConnectionClosed(size_t socketID)
{
    auto it = _connections.find(socketID);
    if (it != _connections.end())
    {
        _connections.erase(it);
    }
}

void libssh_tunnel::threadFunc()
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

