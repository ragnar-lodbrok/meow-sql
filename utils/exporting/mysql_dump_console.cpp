#include "mysql_dump_console.h"
#include "models/forms/export_database_form.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include "helpers/logger.h"

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
    _process->start(program, programArguments());

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

QString MySQLDumpConsole::version() const
{
    QProcess mysqldump;

    mysqldump.start(pathToCommand(), QStringList() << "--version");

    if (!mysqldump.waitForStarted()) {
        return QString();
    }

    if (!mysqldump.waitForFinished()) {
        return QString();
    }

    return QString::fromUtf8(mysqldump.readAll());
}

QString MySQLDumpConsole::currentCommand() const
{
    QString program = pathToCommand();

    const QStringList args = programArguments();

    return program + ' ' + args.join(' ');
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
        args << "--databases" << _form->database();
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

    args << "--verbose";

    args << "-r" << _form->filename(); // --result-file

    return args;
}

QStringList MySQLDumpConsole::connectionOptions() const
{
    QStringList args;

    meow::db::ConnectionParameters * params = _form->session()
            ->connection()->connectionParams();

    args << "-h" << params->hostName();
    args << "-u" << params->userName();
    args << "-p"; // hide password for ps

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

        meow::db::ConnectionParameters * params = _form->session()
                ->connection()->connectionParams();

        _process->write(params->password().toUtf8());
        _process->closeWriteChannel();

        emit progressMessage(QString(QChar::LineFeed)); // Enter

        _passwordEntered = true;

        return true;
    }

    return false;
}

} // namespace exporting
} // namespace utils
} // namespace meow
