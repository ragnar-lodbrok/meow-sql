#include "openssh_tunnel.h"
#include "helpers/logger.h"
#include "db/exception.h"
#include <QApplication>
#include <QThread>
#include <QDebug>
#include <sys/types.h>

#ifdef Q_OS_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

static const quint16 DEFAULT_SSH_PORT = 22;

namespace meow {
namespace ssh {

OpenSSHTunnel::OpenSSHTunnel()
    : _connected(false)
    , _stopWaiting(false)
{

}

OpenSSHTunnel::~OpenSSHTunnel()
{
    disconnect();
}

bool OpenSSHTunnel::connect(const db::ConnectionParameters & params)
{
    _connected = false;
    _stopWaiting = false;
    _lastOutputString = QString();

    _params = params;

    if (!findOpenPort()) {
        return false;
    }

    _process.reset(new QProcess());

    QObject::connect(qApp,
                     &QApplication::aboutToQuit,
                     _process.get(),
                     &QProcess::terminate);

    QObject::connect(
        _process.get(),
        &QProcess::errorOccurred,
        this,
        &OpenSSHTunnel::onProcessError);

    QObject::connect(
        _process.get(),
        &QProcess::readyReadStandardOutput,
        this,
        &OpenSSHTunnel::onProcessStdOut);

    QObject::connect(
        _process.get(),
        &QProcess::readyReadStandardError,
        this,
        &OpenSSHTunnel::onProcessStdErr);

    QObject::connect(
        _process.get(),
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
#else
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
#endif
        [=](int exitCode, QProcess::ExitStatus exitStatus) {
            Q_UNUSED(exitCode);
            Q_UNUSED(exitStatus);
            _stopWaiting = true;
        }
    );

    _process->start(programName(), programArguments());

    _process->waitForStarted();

    while (!_stopWaiting) {
        QThread::msleep(50);
        QCoreApplication::processEvents();
    }

    if (!_connected) {
        failWithError(errorString());
    }

    return _connected;
}

void OpenSSHTunnel::disconnect()
{
    if (_process) {
        _process->terminate();
        _process->waitForFinished();
        _process.reset();
    }
}

bool OpenSSHTunnel::supportsPassword() const
{
#ifdef Q_OS_UNIX
    return true; // via sshpass
#endif
    return false; // no support on win yet
}

SSHTunnelParameters OpenSSHTunnel::params() const
{
    return _params.sshTunnel();
}

QString OpenSSHTunnel::errorString() const
{

    if (_process && _process->error() != QProcess::ProcessError::UnknownError) {
        if (_process->error() == QProcess::ProcessError::FailedToStart) {
            return QObject::tr("Failed to start `ssh` process");
        } else {
            return QObject::tr("Error in `ssh` process");
        }
    }

    if (_connected || _lastOutputString.isEmpty()) {
        return QString();
    }

    return _lastOutputString.split("\r\n", QString::SkipEmptyParts).last();
}

void OpenSSHTunnel::onProcessError(QProcess::ProcessError error)
{
    Q_UNUSED(error);
    _stopWaiting = true;
}

void OpenSSHTunnel::onProcessStdOut()
{
    QByteArray bytes = _process->readAllStandardOutput();
    QString output = QString::fromUtf8(bytes);

    processOutput(output);
}

void OpenSSHTunnel::onProcessStdErr()
{
    QByteArray bytes = _process->readAllStandardError();
    QString error = QString::fromUtf8(bytes);

    processOutput(error);
}

void OpenSSHTunnel::failWithError(const QString & error)
{
    meowLogC(Log::Category::Error)
            << "SSH Tunnel failed: "
            << error;
    QString errorStr = QObject::tr("SSH Tunnel failed: ") + error;

    throw db::Exception(errorStr);
}

void OpenSSHTunnel::processOutput(const QString & output)
{
    _lastOutputString = output;

    if (output.contains("Entering interactive session.")) {
        _connected = true;
        _stopWaiting = true;
    }
}

QString OpenSSHTunnel::programName() const
{
#ifdef Q_OS_WIN
    return "cmd";
#else
    if (supportsPassword() && !_params.sshTunnel().password().isEmpty()) {
        return "sshpass";
    } else {
        return "ssh";
    }
#endif
}

QStringList OpenSSHTunnel::programArguments() const
{
    // sshpass -p !4u2tryhack ssh username@host.example.com
    // ssh -N -L 3308:127.0.0.1:3306 -p 22 root@192.168.1.10

    // TODO ssh -o StrictHostKeyChecking=no or confirmation?

    QStringList args;

#ifdef Q_OS_WIN
    args << "/c";
    args << "ssh";
#endif

    // TODO: add passphrase support?

    if (supportsPassword() && !_params.sshTunnel().password().isEmpty()) {
        // TODO: passing password to cmd is unsafe but I don't care since
        // most "normal" users will use keys <or single-user machines =) >.
        args << "-p"; // param of sshpass!
        args << _params.sshTunnel().password();
        args << "ssh";
        args << "-o";
        args << "PreferredAuthentications=password";
        args << "-o";
        args << "PubkeyAuthentication=no"; // TODO: rm?
    }

    args << "-v"; // to detect successfull connection
    args << "-N"; // no exec, just ports forwarding
    args << "-L"; // ports forwarding
    args << QString("%1:%2:%3").arg(_params.sshTunnel().localPort())
                               .arg(_params.hostName())
                               .arg(_params.port());
    if (_params.sshTunnel().port() != DEFAULT_SSH_PORT) {
        args << "-p" << QString::number(_params.sshTunnel().port());
    }

    args << QString("%1@%2").arg(_params.sshTunnel().user())
                            .arg(_params.sshTunnel().host());

    return args;
}

bool OpenSSHTunnel::findOpenPort()
{
    int portChecks = 0;

    while (!isPortOpen(_params.sshTunnel().localPort())) {

        qDebug() << "SSH: port" << _params.sshTunnel().localPort() << "is busy";

        ++portChecks;
        if (portChecks >= 20) {
            failWithError(
             QObject::tr("Could not execute ssh: Port %1 already in use.")
                        .arg(_params.sshTunnel().localPort()));
            return false;
        }

        _params.sshTunnel().setLocalPort(
                    _params.sshTunnel().localPort() + 1);
    }

    return true;
}

bool OpenSSHTunnel::isPortOpen(unsigned short port)
{
#ifdef Q_OS_UNIX
    sockaddr_in sockAddr;

    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        qDebug() << "Invalid socket";
        return false;
    }

    int res = ::bind(sock,
                     (struct sockaddr *)&sockAddr,
                     sizeof(sockaddr_in));
    ::close(sock);

    return res == 0;
#endif
#ifdef Q_OS_WIN
    return true; // TODO: impl
#endif
}

} // namespace ssh
} // namespace meow
