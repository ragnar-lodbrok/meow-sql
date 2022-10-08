#include "libssh_connection.h"
#include "sockets/connection.h"
#include <QDebug>
#include <utility>

namespace meow {
namespace ssh {

LibSSHConnection::LibSSHConnection(
        db::ConnectionParameters params,
        std::shared_ptr<LibSSH> session,
        std::unique_ptr<LibSSHChannel> channel,
        std::shared_ptr<sockets::Connection> connection,
        const std::shared_ptr<sockets::ISocketReceiver>& receiver)

    : _params(std::move(params))
    , _session(std::move(session))
    , _channel(std::move(channel))
    , _connection(std::move(connection))
    , _receiver(receiver)
    , _stopThread(false)
{

}

LibSSHConnection::~LibSSHConnection()
{
    qDebug() << "[STOP!] DESTRUCTING";
    _stopThread = true;
    if (_thread.joinable()) {
        _thread.join();
    }

    _channel.reset();
    _session.reset();
}

void LibSSHConnection::onError(std::error_code code)
{
    qDebug() << "Socket error: " << QString::fromStdString(code.message());
}

void LibSSHConnection::onClose()
{
    if (auto receiver = _receiver.lock()) {
        receiver->onConnectionClosed(_connection->connectionID());
    }
}

void LibSSHConnection::onData(const std::vector<char>& data, size_t dataLength)
{
    // Data from TCP socket to send to ssh
    _channel->write(data.data(), dataLength);
}

void LibSSHConnection::startTunnel()
{
    std::unique_lock<std::mutex> lk(_threadMutex);
    if (_threadRunning)
    {
        throw std::runtime_error("Thread already running");
    }
    _thread = std::thread(&LibSSHConnection::threadFunc, this);
    _threadWait.wait(lk, [&]{
        return _threadRunning;
    });
}

void LibSSHConnection::threadFunc()
{
    {
        std::unique_lock<std::mutex> lk(_threadMutex);
        _threadRunning = true;
        _threadWait.notify_all();
    }

    _channel->setBlocking(0);
    int rc = SSH_ERROR;
    std::size_t timePassedMs = 0;
    while (timePassedMs < (5 * 1000)) {
        rc = openForward(_channel);
        if (rc == SSH_AGAIN) {
            timePassedMs += 100;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else if (rc == SSH_ERROR) {
            qWarning() << "Error opening channel: " << _session->getError();
            break;
        } else {
            break;
        }
    }
    if (rc != SSH_OK) {
        _threadRunning = false;
        return;
    }

    std::vector<char> buff(1024, '\0');
    while (!_stopThread) {
        int polled = _channel->pollForMs(0, 50);

        if (polled == SSH_ERROR) {
            _stopThread = true;
            break;
        }

        if (polled == 0) {
            continue;
        }

        size_t readlen = 0;
        if ((readlen = _channel->read(buff.data(), buff.size(), false)) == SSH_ERROR) {
            _stopThread = true;
            break;
        }

        if (readlen == 0) {
            continue;
        }

        _connection->write(buff, readlen);
    }
}

int LibSSHConnection::openForward(const std::unique_ptr<LibSSHChannel> &channel)
{
    QByteArray remoteHostBytes = _params.hostName().toLatin1();
    const char* remoteHost = remoteHostBytes.constData();

    QByteArray localHostBytes = QString("127.0.0.1").toLatin1();
    const char* localHost = localHostBytes.constData();

    int remotePort = _params.port();
    int localPort = _params.sshTunnel().localPort();

    qDebug() << "forward 127.0.0.1:" + QString::number(localPort)
             << "=>" << _params.sshTunnel().host() + ":" + QString::number(_params.sshTunnel().port())
             << "=>" << _params.hostName() + ":" + QString::number(remotePort);

    int rc = channel->openForward(remoteHost,
                                  remotePort,
                                  localHost,
                                  localPort);

    return rc;
}

void LibSSHConnection::close()
{
    qDebug() << "[STOP!] CLOSE";
    _stopThread = true;
    if (_thread.joinable()) {
        _thread.join();
    }
}

} // namespace ssh
} // namespace meow
