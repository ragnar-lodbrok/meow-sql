#include "openssh_tunnel.h"
#include "helpers/logger.h"
#include "db/exception.h"
#include <QApplication>
#include <QThread>
#include <QDebug>

static const quint16 DEFAULT_SSH_PORT = 22;

namespace meow {
namespace ssh {

ISSHTunnel::~ISSHTunnel()
{

}

//------------------------------------------------------------------------------

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

    _process->start("ssh", programArguments());

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

QStringList OpenSSHTunnel::programArguments() const
{
    // ssh -N -L 3308:127.0.0.1:3306 -p 22 root@192.168.1.10

    QStringList args;

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

} // namespace ssh
} // namespace meow
