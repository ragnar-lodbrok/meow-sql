/**
 * Based on https://github.com/mysql/mysql-workbench/blob/8.0/library/ssh
 */
#include "libssh_tunnel.h"
#include "helpers/logger.h"
#include "db/exception.h"
#include <fcntl.h>
#include <thread>

#ifndef Q_OS_WIN
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <QDebug>

namespace meow {
namespace ssh {

void failWithError(const QString & error)
{
    meowLogC(Log::Category::Error)
            << "SSH Tunnel failed: "
            << error;
    QString errorStr = QObject::tr("SSH Tunnel failed: ") + error;

    throw db::Exception(errorStr);
}

QString socketErrorString()
{
    return QString::fromUtf8(strerror(errno));
}

void closeSocket(int socketHandle)
{
    // TODO: win?
    close(socketHandle);
}

void makeSocketNonBlocking(int socketHandle)
{
    // TODO: win?
    if (fcntl(socketHandle, F_SETFL, fcntl(socketHandle, F_GETFL, 0) | O_NONBLOCK) == -1) {
        closeSocket(socketHandle);
        failWithError(socketErrorString());
    }
}

inline int pollSockets(pollfd * fds, size_t fdNum, int timeout)
{
    // TODO: win?
    return poll(fds, static_cast<nfds_t>(fdNum), timeout);
}

LibSSHTunnel::LibSSHTunnel()
    : _session(nullptr)
    , _forwardingChannel(nullptr)
{

}

LibSSHTunnel::~LibSSHTunnel()
{
    disconnect();
}

bool LibSSHTunnel::connect(const db::ConnectionParameters & params)
{
    _params = params;

    _session = ssh_new();

    if (_session == nullptr) {
        failWithError("Unable to create SSH session");
        return false;
    }

    int verbosity = SSH_LOG_PROTOCOL;
    int port = 22;

    // TODO: why toLatin1 not toUtf8 ?
    QByteArray hostBytes = _params.sshTunnel().host().toLatin1();
    QByteArray userBytes = _params.sshTunnel().user().toLatin1();
    const char * host = hostBytes.constData();
    const char * user = userBytes.constData();

    ssh_options_set(_session, SSH_OPTIONS_HOST, host);
    ssh_options_set(_session, SSH_OPTIONS_USER, user);
    ssh_options_set(_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    ssh_options_set(_session, SSH_OPTIONS_PORT, &port);

    qDebug() << "Connecting" << host << user;

    if (ssh_connect(_session) != SSH_OK) {
        failWithError(errorString());
        return false;
    }

    if (!verifyHost()) {
        failWithError("Unable to verify SSH host");
        return false;
    }

    if (!_params.password().isEmpty()) {
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

    /*if (!openForward()) {
        failWithError(errorString());
        return false;
    }*/

    _wakeupSocket = createSocket();

    qDebug() << "Socket created";

    _stopThread = false; // TODO: here?

    if (pthread_create(&_thread, nullptr, &LibSSHTunnel::threadFunc, this) != 0) {
        failWithError("Unable to create thread");
    }

    //closeSocket(_wakeupSocket); // test

    std::this_thread::sleep_for(std::chrono::milliseconds(1000*5));

    qDebug() << "Here!";

    return true;
}

void LibSSHTunnel::disconnect()
{
    if (_forwardingChannel) {
        ssh_channel_free(_forwardingChannel);
        _forwardingChannel = nullptr;
    }

    if (_session) {
        ssh_disconnect(_session);
        ssh_free(_session);
        _session = nullptr;
    }
}

QString LibSSHTunnel::errorString() const
{
    if (_session) {
        return QString(ssh_get_error(_session));
    }
    return {};
}

bool LibSSHTunnel::verifyHost()
{
    // https://api.libssh.org/stable/libssh_tutor_guided_tour.html#serverauth

    // TODO: complete

    enum ssh_known_hosts_e state;

    state = ssh_session_is_known_server(_session);

    return state == SSH_KNOWN_HOSTS_OK;
}

bool LibSSHTunnel::authWithPassword()
{
    // The 'password' value MUST be encoded UTF-8

    qDebug() << "Auth with password:" << _params.sshTunnel().password();

    QByteArray passwordBytes = _params.sshTunnel().password().toUtf8();
    const char * password = passwordBytes.constData();

    int rc = ssh_userauth_password(_session, nullptr, password);
    return rc == SSH_AUTH_SUCCESS;
}

bool LibSSHTunnel::authWithPublicKey()
{
    // TODO: not tested/not complete

    int rc = ssh_userauth_publickey_auto(_session, nullptr, nullptr);

    return rc == SSH_AUTH_SUCCESS;
}

int LibSSHTunnel::openForward()
{
    /*_forwardingChannel = ssh_channel_new(_session);
    if (_forwardingChannel == nullptr) {
        return false;
    }*/

    QByteArray remoteHostBytes = _params.sshTunnel().host().toLatin1();
    const char * remoteHost = remoteHostBytes.constData();

    QByteArray localHostBytes = _params.hostName().toLatin1();
    const char * localHost = localHostBytes.constData();

    int remotePort = _params.port();
    int localPort = _params.sshTunnel().localPort();

    qDebug() << "forward" << _params.sshTunnel().host() + ":" + QString::number(remotePort)
             << "=>" << _params.hostName() + ":" + QString::number(localPort);

    int rc = ssh_channel_open_forward(_forwardingChannel,
                                      remoteHost,
                                      remotePort,
                                      localHost,
                                      localPort);

    // https://stackoverflow.com/a/43578529
    // https://github.com/mysql/mysql-workbench/blob/8.0/library/ssh/SSHTunnelManager.cpp

    return rc;
}

int LibSSHTunnel::createSocket()
{
    int socketHandle = socket(AF_INET, SOCK_STREAM, 0);
    if (socketHandle == -1) {
        failWithError(socketErrorString());
    }

    int val = 1;
    if (setsockopt(socketHandle, SOL_SOCKET, SO_REUSEADDR,
                   (char *)&val, sizeof(val)) == -1) {

        closeSocket(socketHandle);
        failWithError(socketErrorString());
    }

    makeSocketNonBlocking(socketHandle);

    QByteArray localHostBytes = _params.hostName().toLatin1();
    const char * localHost = localHostBytes.constData();
    uint16_t localPort = _params.sshTunnel().localPort();


    struct sockaddr_in addr;
    socklen_t len = sizeof(struct sockaddr_in);
    memset(&addr, 0, len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(localHost);
    addr.sin_port = htons(0); // localPort

    // TODO: try to bind to increased port numbers several times
    // in case localPort is taken by another session

    qDebug() << "Socket bind: " << localHost << ":" << localPort;

    if (bind(socketHandle, (struct sockaddr*) &addr, len) == -1) {
        closeSocket(socketHandle);
        failWithError(socketErrorString());
    }

    getsockname(socketHandle, (struct sockaddr*) &addr, &len);
    int bindPort = htons(addr.sin_port);

    qDebug() << "Binded to port: " << bindPort;

    _params.sshTunnel().setLocalPort(bindPort);

    if (listen(socketHandle, 5) == -1) {
        closeSocket(socketHandle);
        failWithError(socketErrorString());
    }

    return socketHandle;
}

void* LibSSHTunnel::threadFunc()
{

    int rc = 0;
    fdsNumber = 0;

    memset(fds, 0 , sizeof(fds));

    fds[0].fd = _wakeupSocket;
    fds[0].events = POLLIN;
    ++fdsNumber;

    do {

        qDebug() << "poll" << fdsNumber;
        rc = pollSockets(fds, fdsNumber, 1000 * 60);
        qDebug() << "poll end";

        if (rc < 0) { // error
            qDebug() << "Socket error:" << rc;
            break;
        } else if (rc == 0) { // timeout
            qDebug() << "Socket timeout:" << rc;
            //break;
        }

        size_t polledFdsNumber = fdsNumber; // copy to change in for
        for (size_t i = 0; i < polledFdsNumber; ++i) {

            const pollfd & itFd = fds[i];

            if (itFd.revents == 0) {
                continue;
            }

            if (itFd.revents == POLLERR) {
                qDebug() << "Error poll";
                _stopThread = true;
                break;
            }

            if (itFd.fd == _wakeupSocket) { // new connection
                qDebug() << "New socket connection!";

                if (polledFdsNumber == 3) {
                    qDebug() << "Too much connections!";

                    struct sockaddr_in client;
                    socklen_t addrlen = sizeof(client);
                    int sock = accept(_wakeupSocket, (struct sockaddr*)&client, &addrlen);
                    closeSocket(sock);

                    continue;
                }

                if (!acceptNewSocket()) {
                    break;
                }

                if (!openTunnel()) {
                    break;
                }
            } else if (itFd.fd == _clientSocket) { // read from client socket

                forwardToSSH();

                if (_stopThread) break;

            } else { // read from ssh channel

                forwardFromSSH2();

            }
        }

        if (_stopThread) qDebug() << "STOP!!!";

    } while(!_stopThread);

    pthread_exit(0);
    return nullptr;
}

void* LibSSHTunnel::threadFunc(void* data)
{
    return static_cast<LibSSHTunnel *>(data)->threadFunc();
}

bool LibSSHTunnel::acceptNewSocket()
{
    struct sockaddr_in client;
    socklen_t addrlen = sizeof(client);
    _clientSocket = accept(_wakeupSocket, (struct sockaddr*)&client, &addrlen);
    if (_clientSocket < 0) {
        if (errno != EWOULDBLOCK || errno == EINTR) {
            qDebug() << "Failed to accept new socket: " << errno;
            _stopThread = true;
        }
        return false;
    }

    makeSocketNonBlocking(_clientSocket);

    fds[fdsNumber].fd = _clientSocket;
    fds[fdsNumber].events = POLLIN;
    ++fdsNumber;

    qDebug() << "Accepted new socket!";

    return true;
}

bool LibSSHTunnel::openTunnel()
{

    _forwardingChannel = ssh_channel_new(_session);
    if (_forwardingChannel == nullptr) {
        return false;
    }

    ssh_channel_set_blocking(_forwardingChannel, false); // ?

    int rc = SSH_ERROR;
    std::size_t timePassedMs = 0;

    while (timePassedMs < (5 * 1000) && !_stopThread) {

        rc = openForward();
        if (rc == SSH_AGAIN) {
            qDebug() << "Open channel retry";
            timePassedMs += 100;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            qDebug() << "Channel opened";
            break;
        }
    }

    if (rc != SSH_OK) {
        _stopThread = true;
        return false;
    }

    fds[fdsNumber].fd = ssh_get_fd(_session);
    fds[fdsNumber].events = POLLIN;
    ++fdsNumber;

    return true;
}

void LibSSHTunnel::forwardToSSH()
{
    qDebug() << "forwardToSSH";

    ssize_t readlen = 0;
    std::vector<char> buff(1024, '\0');

    while (!_stopThread && (readlen = recv(_clientSocket, buff.data(), buff.size(), 0)) > 0) {
        qDebug() << "Read from client: " << readlen;
      int bWritten = 0;
      for (char* buffPtr = buff.data(); readlen > 0 && !_stopThread; buffPtr += bWritten, readlen -= bWritten) {
          qDebug() << "write " << readlen;
          bWritten = ssh_channel_write (_forwardingChannel, buffPtr, readlen);
          if (bWritten == SSH_ERROR ) {
                _stopThread = true;
                qDebug() << "Error writing to ssh: " << ssh_get_error(_session);
                break;
          }
      }
      qDebug() << "Written to ssh:" << bWritten;
    }

    qDebug() << "forwardToSSH end";
}

void LibSSHTunnel::forwardFromSSH()
{
    qDebug() << "forwardFromSSH";

    ssize_t readlen = 0;
    std::vector<char> buff(1024, '\0');
    do {
        readlen = ssh_channel_read_nonblocking(_forwardingChannel, buff.data(), buff.size(), false);

        qDebug() << "ssh_channel_read_nonblocking" << readlen;

      if (readlen < 0 && readlen != SSH_AGAIN) {
          _stopThread = true;
          qDebug() << "Failed to read ssh channel" << readlen;
          break;
      }


      if (readlen == 0) {
          qDebug() << "stop";
        //if (chan->isClosed())
        //  throw SSHTunnelException("channel is closed");
        break;
      }

      ssize_t bWritten = 0;
      for (char* buffPtr = buff.data(); readlen > 0 && !_stopThread; buffPtr += bWritten, readlen -= bWritten) {
        bWritten = send(_clientSocket, buffPtr, readlen, MSG_NOSIGNAL);
        if (bWritten <= 0) {
            qDebug() << "Failed to write";
            _stopThread = true;
            break;
        }
        qDebug() << "Written to client" << bWritten;
      }
    } while (!_stopThread && true);
}

void LibSSHTunnel::forwardFromSSH2()
{

    qDebug() << "forwardFromSSH2";

    int readlen = 0;

    do {

        readlen = 0;
        qDebug() << "poll2";
        int polled = ssh_channel_poll(_forwardingChannel, 0);
        qDebug() << "poll2 ended" << polled;


        if (polled == SSH_ERROR) {
            qDebug() << "ssh poll error";
            _stopThread = true;
            break;
        }

        if (polled == 0) {
            qDebug() << "polled 0";
            break;
            // TODO:
        }

        std::vector<char> buff(1024, '\0');

        if ((readlen = ssh_channel_read ( _forwardingChannel, buff.data(), buff.size(), false ) ) == SSH_ERROR) {
            _stopThread = true;
            qDebug() << "Failed to read ssh channel" << readlen;
            break;
        }

        qDebug() << "ssh_channel_read" << readlen;

        if (readlen == 0) {
            break;
        }

        int bWritten = send(_clientSocket, buff.data(), readlen, MSG_NOSIGNAL);
        if (bWritten <= 0) {
             qDebug() << "Failed to write";
            _stopThread = true;
            break;
        }

        qDebug() << "written " << bWritten;


    } while (readlen >= 1024);

    qDebug() << "forwardFromSSH2 end";
}

} // namespace ssh
} // namespace meow
