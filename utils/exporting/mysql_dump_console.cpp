#include "mysql_dump_console.h"
#include "models/forms/export_database_form.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include "helpers/logger.h"
#include <QDebug>

namespace meow {
namespace utils {
namespace exporting {

const quint16 MYSQL_DEFAULT_PORT = 3306;

MySQLDumpConsole::MySQLDumpConsole(models::forms::ExportDatabaseForm * form)
    : QObject()
    , _form(form)
    , _passwordEntered(false)
    , _isCancelledbyUser(false)
{

}

MySQLDumpConsole::~MySQLDumpConsole()
{
    cancel();
}

void MySQLDumpConsole::start()
{

    _isCancelledbyUser = false;

    QString program = pathToCommand();

    meowLogC(Log::Category::Debug) << currentCommand();

    emit progressMessage(currentCommand() + QChar::LineFeed);

    _process.reset(new QProcess());

    connect(
        _process.get(),
        &QProcess::errorOccurred,
        [=](QProcess::ProcessError error) {

            meowLogC(Log::Category::Error) << "mysqldump failed, status: "
                                           << static_cast<int>(error);

            QString errString;
            switch (error) {
            case QProcess::ProcessError::FailedToStart:
                errString = "mysqldump failed to start:"
                            " is missing or insufficient permissions";
                break;
            default:
                errString = "mysqldump has finished with error";
            };

            emit progressMessage(errString + QChar::LineFeed);

            emit finished(_isCancelledbyUser ? true : false);
        }
    );

    connect(
        _process.get(),
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
            QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
#else
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
#endif
        [=](int exitCode, QProcess::ExitStatus exitStatus) {

            if (exitStatus != QProcess::ExitStatus::NormalExit) {
                meowLogC(Log::Category::Error)
                    << "mysqldump failed, exit code: "
                    << static_cast<int>(exitCode);
            }

            emit finished(
                QProcess::ExitStatus::NormalExit == exitStatus
                || _isCancelledbyUser);
        }
    );

    connect(
        _process.get(),
        &QProcess::readyReadStandardOutput,
        [=]() {

            QByteArray bytes = _process->readAllStandardOutput();
            QString output = QString::fromUtf8(bytes);

            emit progressMessage(output);

            enterPasswordIfRequested(output);
        }
    );

    connect(
        _process.get(),
        &QProcess::readyReadStandardError,
        [=]() {

            QByteArray bytes = _process->readAllStandardError();
            QString error = QString::fromUtf8(bytes);

            emit progressMessage(error);

            enterPasswordIfRequested(error); // should be in stdin but I get
            // "Enter password: " in stderr
        }
    );

    //_process->start(program, programArguments());
    _process->start(currentCommand());
}

bool MySQLDumpConsole::cancel()
{
    if (isRunning()) {
        _isCancelledbyUser = true;
        _process->kill();
        return true;
    }
    return false;
}

bool MySQLDumpConsole::isRunning() const
{
    return _process && _process->state() != QProcess::NotRunning;
}

QString MySQLDumpConsole::versionString() const
{
    if (!_versionString.isEmpty()) {
        return _versionString;
    }

    QProcess mysqldump;

    mysqldump.start(pathToCommand(), QStringList() << "--version");

    if (!mysqldump.waitForStarted()) {
        return QString();
    }

    if (!mysqldump.waitForFinished()) {
        return QString();
    }

    _versionString = QString::fromUtf8(mysqldump.readAll());
    return _versionString;
}

QVersionNumber MySQLDumpConsole::versionNumber() const
{
    // Examples:
    // * Version 8:
    // mysqldump Ver 8.0.21-0ubuntu0.20.04.4 for Linux on x86_64 ((Ubuntu))
    // * Version 5:
    // mysqldump  Ver 10.13 Distrib 5.7.31, for Linux (x86_64)
    // Older versions have higher numbers?

    QString versionStr = versionString();

    if (versionStr.isEmpty()) {
        return QVersionNumber();
    }

    QStringList versionPrefixes;
    versionPrefixes << "Distrib ";
    versionPrefixes << "mysqldump Ver ";

    for (const QString & versionPrefix : versionPrefixes) {

        int prefixIndex = versionStr.indexOf(versionPrefix);
        if (prefixIndex != -1) {
            QString versionSubstr = versionStr.mid(
                        prefixIndex + versionPrefix.length());
            QVersionNumber number = QVersionNumber::fromString(versionSubstr);
            if (number.majorVersion() > 0) {
                return number;
            }
        }
    }

    return QVersionNumber();
}

QString MySQLDumpConsole::currentCommand() const
{
    QString program = pathToCommand();

    const QStringList args = programArguments();

    return program + ' ' + args.join(' ');
}

bool MySQLDumpConsole::supportsColumnStatisticsOption() const
{
    return versionNumber().majorVersion() >= 8;
}

QString MySQLDumpConsole::pathToCommand() const
{
    return "mysqldump";
}

QStringList MySQLDumpConsole::programArguments() const
{
    QStringList args;

    args << connectionOptions();

    if (_form->allDatabases()) {
        args << "--all-databases";
    } else {
        args << "--databases" << escapeString(_form->database());
    }

    using Option = models::forms::MySQLDumpOption;

    if (_form->isOptionEnabled(Option::AddDropDatabase)) {
        args << "--add-drop-database";
    }
    if (_form->isOptionEnabled(Option::AddDropTrigger)) {
        args << "--add-drop-trigger";
    }
    if (_form->isOptionEnabled(Option::Events)) {
        args << "--events";
    }
    if (_form->isOptionEnabled(Option::Routines)) {
        args << "--routines";
    }
    if (_form->isOptionEnabled(Option::Triggers)) {
        // enabled by default, no need to specify
        //args << "--triggers";
    }

    if (_form->isOptionEnabled(Option::NoCreateDatabase)) {
        args << "--no-create-db";
    }
    if (_form->isOptionEnabled(Option::NoTriggers)) {
        args << "--skip-triggers";
    }
    if (_form->isOptionEnabled(Option::NoCreateTable)) {
        args << "--no-create-info";
    }
    if (_form->isOptionEnabled(Option::NoOptGroup)) {
        args << "--skip-opt";
    }
    // --opt group begin, keep after --skip-opt
    if (_form->isOptionEnabled(Option::AddDropTable)
        && _form->isOptionEnabled(Option::NoOptGroup)) {
        args << "--add-drop-table";
    }
    if (_form->isOptionEnabled(Option::AddLocks)) {
        args << "--add-locks";
    }
    if (_form->isOptionEnabled(Option::CreateOptions)) {
        args << "--create-options";
    }
    if (_form->isOptionEnabled(Option::DisableKeys)) {
        args << "--disable-keys";
    }
    if (_form->isOptionEnabled(Option::ExtendedInsert)) {
        args << "--extended-insert";
    }
    if (_form->isOptionEnabled(Option::LockTables)) {
        args << "--lock-tables";
    }
    if (_form->isOptionEnabled(Option::Quick)) {
        args << "--quick";
    }
    if (_form->isOptionEnabled(Option::SetCharset)) {
        args << "--set-charset";
    }
    // --opt group end

    if (_form->isOptionEnabled(Option::NoColumnStatistics)) {
        args << "--column-statistics=0";
    }

    args << "--verbose";

    args << "-r" << escapeString(_form->filename()); // --result-file

    return args;
}

QStringList MySQLDumpConsole::connectionOptions() const
{
    QStringList args;

    meow::db::ConnectionParameters * params = _form->session()
            ->connection()->connectionParams();

    args << "-h" << params->hostName();
    args << "-u" << params->userName();
#ifdef Q_OS_WIN
    // TODO: export MYSQL_PWD=*** ?
    // Can't make input password work on win
    args << "--password=" + escapeString(password());
#else
    args << "-p"; // hide password for ps
#endif

    if (params->port() != MYSQL_DEFAULT_PORT) {
        args << "--port=" + QString::number(params->port());
    }

    return args;
}

bool MySQLDumpConsole::enterPasswordIfRequested(const QString & output)
{
    if (_passwordEntered) {
        return false;
    }

    // I hope the prompt is same everywhere and we get full row
    if (output.contains(QString("Enter password: "))) {

        _process->write(this->password().toUtf8());
        _process->closeWriteChannel();

        emit progressMessage(QString(QChar::LineFeed)); // Enter

        _passwordEntered = true;

        return true;
    }

    return false;
}

QString MySQLDumpConsole::password() const
{
    meow::db::ConnectionParameters * params = _form->session()
            ->connection()->connectionParams();
    return params->password();
}

QString MySQLDumpConsole::escapeString(const QString & str) const
{
    const QChar quote = '"';
    const QChar backslash = '\\';
    const QString escapedQuote = QString(backslash) + QString(quote);
    QString string = str;
    return QString(quote) + string.replace(quote, escapedQuote) + QString(quote);
}

} // namespace exporting
} // namespace utils
} // namespace meow
