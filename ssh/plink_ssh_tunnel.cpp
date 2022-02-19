#include "plink_ssh_tunnel.h"
#include "helpers/logger.h"
#include "db/exception.h"
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include <QInputDialog>
#include <QDir>
#include <Windows.h>
#include <namedpipeapi.h>

// Known hosts location for plink:
// HKEY_CURRENT_USER\Software\SimonTatham\PuTTY\SshHostKeys

static const quint16 DEFAULT_SSH_PORT = 22;

namespace meow {
namespace ssh {

struct ProcessPipe
{
    HANDLE readHandle;
    HANDLE writeHandle;
    bool created = false;

    ProcessPipe()
    {
        if (!CreatePipe(&readHandle, &writeHandle, NULL, 8192)) {
            return;
        }
        _closeReadHandle = true;
        _closeWriteHandle = true;

        if (!DuplicateHandle(
              GetCurrentProcess(), readHandle,
              GetCurrentProcess(), &readHandle, 0, TRUE,
              DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS)) {
            _closeReadHandle = false;
            return;
        }

        if (!DuplicateHandle(
              GetCurrentProcess(), writeHandle,
              GetCurrentProcess(), &writeHandle, 0, TRUE,
              DUPLICATE_CLOSE_SOURCE | DUPLICATE_SAME_ACCESS)) {
            _closeWriteHandle = false;
            return;
        }

        created = true;
    }

    ~ProcessPipe()
    {
        if (_closeReadHandle) {
            CloseHandle(readHandle);
        }
        if (_closeWriteHandle) {
            CloseHandle(writeHandle);
        }
    }
private:
    bool _closeReadHandle = false;
    bool _closeWriteHandle = false;
};

class PLinkSSHTunnel::Impl
{
private:
    ProcessPipe _inPipe;
    ProcessPipe _outPipe;
    ProcessPipe _errorPipe;
    PROCESS_INFORMATION _processInformation;
    bool _processCreated = false;
    db::ConnectionParameters _params;

    // winsock for port open checks
    WORD _wsaVersionRequested;
    WSADATA _wsaData;
    bool _wsaStarted = false;

public:
    Impl()
    {
        _wsaVersionRequested = MAKEWORD(2, 2);

        int err = WSAStartup(_wsaVersionRequested, &_wsaData);
        if (err != 0) {
            qDebug() << "WSAStartup failed: " << err;
        } else {

            if (LOBYTE(_wsaData.wVersion) != 2
                    || HIBYTE(_wsaData.wVersion) != 2) {
                qDebug() << "Could not find a usable version of Winsock.dll";
                WSACleanup();
            } else {
                _wsaStarted = true;
            }
        }
    }

    ~Impl()
    {
        if (_wsaStarted) {
            WSACleanup();
        }
    }

    bool connect(const db::ConnectionParameters & params)
    {
        _params = params;

        if (!findOpenPort()) {
            return false;
        }

        if (!createProcess()) {
            return false;
        }

        if (!waitStarted()) {
            return false;
        }

        return true;
    }

    void disconnect()
    {
        if (_processCreated) {
            // TODO: ExitProcess() ?
            TerminateProcess(_processInformation.hProcess, 0);
            CloseHandle(_processInformation.hProcess);
            CloseHandle(_processInformation.hThread);
            _processCreated = false;
        }
    }
private:

    bool findOpenPort()
    {
        int portChecks = 0;

        while (!isPortOpen(_params.sshTunnel().port())) {
            ++portChecks;
            if (portChecks >= 20) {
                failWithError(
                 QObject::tr("Could not execute PLink: Port %1 already in use.")
                            .arg(_params.sshTunnel().port()));
                return false;
            }

            _params.sshTunnel().setPort(_params.sshTunnel().port() + 1);
        }

        return true;
    }

    bool isPortOpen(unsigned short port)
    {
        if (!_wsaStarted) return true;

        sockaddr_in sockAddr;

        sockAddr.sin_family = AF_INET;
        sockAddr.sin_port = htons(port);
        sockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

        if (sock == INVALID_SOCKET) {
            qDebug() << "Invalid socket";
            return false;
        }

        int res = ::bind(sock,
                         (PSOCKADDR)&sockAddr,
                         sizeof(sockaddr_in));

        if (res == SOCKET_ERROR) {
            //qDebug() << "Port:" << port << "error:" << WSAGetLastError();
        }

        return res == 0;
    }

    bool createProcess()
    {
        if (!_inPipe.created || !_outPipe.created || !_errorPipe.created) {
            failWithError(QObject::tr("Error creating I/O pipes"));
            return false;
        }

        STARTUPINFO startupInfo;
        ZeroMemory(&startupInfo, sizeof(startupInfo));

        startupInfo.cb = sizeof(startupInfo);
        startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
        startupInfo.wShowWindow = SW_HIDE;
        startupInfo.hStdInput = _inPipe.readHandle;
        startupInfo.hStdError = _errorPipe.writeHandle;
        startupInfo.hStdOutput = _outPipe.writeHandle;

        ZeroMemory(&_processInformation, sizeof(_processInformation));

        QString appName = "plink.exe";
        QString commandLine = programArguments().join(' ');
        QByteArray commandLineArr = commandLine.toUtf8();

        if (!CreateProcess(
             appName.toUtf8(),
             commandLineArr.data(),
             NULL,
             NULL,
             TRUE,
             CREATE_DEFAULT_ERROR_MODE
                    | CREATE_NEW_CONSOLE
                    | NORMAL_PRIORITY_CLASS,
             NULL,
             NULL,
             &startupInfo,
             &_processInformation))
        {
            failWithError(
                QObject::tr("Could not execute PLink: %1")
                        .arg(lastWindowsErrorAsString()));

            return false;
        }

        _processCreated = true;

        return true;
    }

    bool waitStarted()
    {
        int sshTimeoutSec = 4;

        int waited = 0;
        int returnedSomethingAt = -1;
        int waitStep = 50;

        DWORD processExitCode;

        while (waited < sshTimeoutSec * 1000) {

            waited += waitStep;

            WaitForSingleObject(_processInformation.hProcess, waitStep);
            GetExitCodeProcess(_processInformation.hProcess, &processExitCode);
            if (processExitCode != STILL_ACTIVE) {
                failWithError(QObject::tr("PLink exited unexpected."));
                return false;
            }

            QString outText = readPipe(&_outPipe);
            QString errorText = readPipe(&_errorPipe);

            if (!outText.isEmpty()) {
                returnedSomethingAt = waited;
                if (!processOutput(outText)) {
                    return false;
                }
            }

            if (!errorText.isEmpty()) {
                returnedSomethingAt = waited;
                if (!processError(errorText)) {
                    return false;
                }
            }

            // Exit loop after 2s idletime when there was output earlier
            const int idleTimeout = 2000;
            if ((returnedSomethingAt > 0)
                    && (waited >= returnedSomethingAt + idleTimeout)) {
                break;
            }

            QCoreApplication::processEvents();
        }

        return true;
    }

    QStringList programArguments() const
    {
        // plink bob@domain.com -pw myPassw0rd1 -P 22 \
        // -i "keyfile.pem" -L 55555:localhost:3306 -batch

        QStringList args;

        args << "-ssh"; // force ssh protocol

        args << QString("%1@%2").arg(_params.sshTunnel().user())
                                .arg(_params.sshTunnel().host());

        QString password = _params.sshTunnel().password();
        if (!password.isEmpty()) {
            args << "-pw"; // password
            // Escape double quote with backslash
            password.replace(QLatin1Char('"'), QLatin1String("\\\""));
            args << '"' + password + '"';
        }

        if (_params.sshTunnel().port() != DEFAULT_SSH_PORT) {
            args << "-P" << QString::number(_params.sshTunnel().port());
        }

        // TODO: private key

        args << "-N"; // don't start a shell/command

        args << "-L"; // ports forwarding
        args << QString("%1:%2:%3").arg(_params.sshTunnel().localPort())
                                   .arg(_params.hostName())
                                   .arg(_params.port());

        //args << "-batch"; // test

        return args;
    }

    void failWithError(const QString & message)
    {
        meowLogC(Log::Category::Error)
                << "SSH Tunnel failed: "
                << message;
        QString errorStr = QObject::tr("SSH Tunnel failed: ") + message;

        throw db::Exception(errorStr);
    }

    QString lastWindowsErrorAsString()
    {
        DWORD errorId = GetLastError();
        if (errorId == 0) {
            return QString();
        }

        LPSTR buff = nullptr;

        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER
                    | FORMAT_MESSAGE_FROM_SYSTEM
                    | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            errorId,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
            (LPSTR)&buff,
            0,
            NULL);

        QString message = QString::fromUtf8(buff, size);

        LocalFree(buff);

        return message;
    }

    QString readPipe(ProcessPipe * pipe)
    {
        if (pipe->readHandle == INVALID_HANDLE_VALUE) {
            failWithError(QObject::tr("Error reading I/O pipes"));
            return QString();
        }

        DWORD bufferReadCount = 0;
        PeekNamedPipe(pipe->readHandle, NULL, 0,
                      NULL, &bufferReadCount, NULL);

        std::string resultBuffer;

        if (bufferReadCount > 0) {

            const int chunkSize = 1024;

            CHAR buffer[chunkSize];
            ZeroMemory(&buffer, sizeof(buffer));

            DWORD bytesRemaining = bufferReadCount;

            while (bytesRemaining >= chunkSize) {
                ReadFile(pipe->readHandle, &buffer,
                         chunkSize, &bufferReadCount, NULL);
                bytesRemaining -= bufferReadCount;

                resultBuffer += std::string(buffer, bufferReadCount);
            }

            if (bytesRemaining > 0) {
                ReadFile(pipe->readHandle, &buffer,
                         bytesRemaining, &bufferReadCount, NULL);
                resultBuffer += std::string(buffer, bufferReadCount);
            }

        }

        QString result = QString::fromStdString(resultBuffer);

        // TODO: H: CleanEscSeq ?

        result.replace("\r\r\n", "\r\n"); // TODO: why?
        return result;
    }

    bool processOutput(const QString & output)
    {
        QString promptUsernameRegexpStr = QString(R"(login as\s*\:)");
        QRegularExpression promptUsernameRegexp(promptUsernameRegexpStr);
        QRegularExpressionMatch usernameRegexpMatch
                = promptUsernameRegexp.match(output);

        if (usernameRegexpMatch.hasMatch()) {
            bool ok;
            QString username = QInputDialog::getText(
                        nullptr,
                        tr("PLink:"),
                        output,
                        QLineEdit::Normal,
                        QDir::home().dirName(),
                        &ok);

            if (!ok) return false;
            sendString(username + "\r\n");
            return true;
        }

        QString promptpasswordRegexpStr
                = QString(R"((password|Passphrase for key "[^"]+")\s*\:)");
        QRegularExpression promptPasswordRegexp(promptpasswordRegexpStr);
        QRegularExpressionMatch passwordRegexpMatch
                = promptPasswordRegexp.match(output);

        if (passwordRegexpMatch.hasMatch()) {
            bool ok;
            QString password = QInputDialog::getText(
                        nullptr,
                        tr("PLink:"),
                        output,
                        QLineEdit::Password,
                        QString(),
                        &ok);

            if (!ok) return false;
            sendString(password + "\r\n");

            return true;
        }

        if (output.trimmed().isEmpty()) return true;

        QMessageBox msgBox;
        msgBox.setText(output);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();

        return true;
    }

    bool processError(const QString & error)
    {
        QString promptRegexpStr
                = QString(R"(([^\.]+\?)(\s*\(y\/n\s*(,[^\)]+)?\)\s*)$)");

        QRegularExpression promptRegexp(promptRegexpStr,
            QRegularExpression::CaseInsensitiveOption
            | QRegularExpression::MultilineOption
            | QRegularExpression::DotMatchesEverythingOption
            | QRegularExpression::InvertedGreedinessOption);

        QRegularExpressionMatch regexpMatch
                = promptRegexp.match(error);

        if (regexpMatch.hasMatch()) {

            QString questionTitle = regexpMatch.captured(1).trimmed();
            QString questionDetails = error.left(
                        error.size() - regexpMatch.capturedLength(2) );

            QMessageBox msgBox;
            msgBox.setText(questionTitle);
            msgBox.setInformativeText(questionDetails);
            msgBox.setStandardButtons(QMessageBox::Yes
                                      | QMessageBox::No
                                      | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Question);
            int confirmStatus = msgBox.exec();

            switch (confirmStatus) {
            case QMessageBox::Yes:
                sendString("y");
                break;
            case QMessageBox::No:
                sendString("n");
                break;
            case QMessageBox::Cancel:
                return false;
            default:
                break;
            }
        } else if (error.startsWith("Using username ",
                             Qt::CaseInsensitive)
                || error.startsWith("Pre-authentication banner ",
                                    Qt::CaseInsensitive)) {

            sendString("\r\n");

        } else if (error.contains("Access denied")) {
            failWithError(error);
            return false;
        } else {
            QMessageBox msgBox;
            msgBox.setText(error);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
        }

        return true;
    }

    DWORD sendString(const QString & str)
    {
        if (str.isEmpty()) return 0;

        QByteArray buffer = str.toUtf8();

        DWORD writtenBytes = 0;
        if (WriteFile(_inPipe.writeHandle, buffer.data(),
                buffer.length(), &writtenBytes, NULL)) {
            return writtenBytes;
        }
        return 0;
    }
};


PLinkSSHTunnel::PLinkSSHTunnel()
    : _impl(new Impl())
{

}

PLinkSSHTunnel::~PLinkSSHTunnel()
{
    _impl->disconnect();
    delete _impl;
}

bool PLinkSSHTunnel::connect(const db::ConnectionParameters & params)
{
    return _impl->connect(params);
}

void PLinkSSHTunnel::disconnect()
{
    _impl->disconnect();
}

} // namespace ssh
} // namespace meow
