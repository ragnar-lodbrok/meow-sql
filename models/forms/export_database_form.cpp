#include "export_database_form.h"
#include "db/connection.h"
#include "db/entity/session_entity.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>

namespace meow {
namespace models {
namespace forms {

ExportDatabaseForm::ExportDatabaseForm(db::SessionEntity * session)
    : _session(session)
    , _allDatabases(true)
    , _filename(generateFilename())
    , _options(0)
{
    resetOptionsToDefault();
}

const QStringList ExportDatabaseForm::databases() const
{
    QStringList list;
    list << QObject::tr("All databases");
    list << _session->connection()->allDatabases();
    return list;
}

QString ExportDatabaseForm::generateFilename() const
{
    // TODO: more advanced
    return QDir::toNativeSeparators(
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + QDir::separator() + "dump.sql"
    );
}

void ExportDatabaseForm::startExport()
{
    _dumper.reset(new meow::utils::exporting::MySQLDumpConsole(this));

    connect(_dumper.get(),
            &meow::utils::exporting::MySQLDumpConsole::finished,
            this,
            &ExportDatabaseForm::finished);

    connect(_dumper.get(),
            &meow::utils::exporting::MySQLDumpConsole::progressMessage,
            this,
            &ExportDatabaseForm::progressMessage);

    _dumper->start();
}

bool ExportDatabaseForm::cancelExport()
{
    if (_dumper) {
        return _dumper->cancel();
    }
    return false;
}

void ExportDatabaseForm::setOptionPrivate(MySQLDumpOption opt, bool enabled)
{
    if (enabled) {
        _options |= static_cast<uint32_t>(opt);
    } else {
        _options &= ~static_cast<uint32_t>(opt);
    }
}

void ExportDatabaseForm::setOption(MySQLDumpOption opt, bool enabled)
{
    uint32_t prevOptionsValue = _options;

    setOptionPrivate(opt, enabled);

    if (prevOptionsValue == _options) {
        return;
    }

    switch (opt) {

    case MySQLDumpOption::AllDatabases: // --all-databases
        // either --all-databases or --databases
        setOptionPrivate(MySQLDumpOption::Databases, !enabled);
        break;

    case MySQLDumpOption::Databases: // --databases
        // either --all-databases or --databases
        setOptionPrivate(MySQLDumpOption::AllDatabases, !enabled);
        break;

    // <----------------------------
    case MySQLDumpOption::CreateDatabase:
        // on when --all-databases or --databases
        setOptionPrivate(MySQLDumpOption::NoCreateDatabase, !enabled);
        break;
    case MySQLDumpOption::NoCreateDatabase: //  --no-create-db,
        // Suppress the CREATE DATABASE statements that are otherwise included
        // in the output if the --databases or --all-databases option is given.
        setOptionPrivate(MySQLDumpOption::CreateDatabase, !enabled);
        break;
    // ----------------------------->


    // <----------------------------
    case MySQLDumpOption::CreateTable:
        setOptionPrivate(MySQLDumpOption::NoCreateTable, !enabled);
        break;

    case MySQLDumpOption::NoCreateTable: //  --no-create-info
        setOptionPrivate(MySQLDumpOption::CreateTable, !enabled);
        break;
    // ----------------------------->


    // <----------------------------
    case MySQLDumpOption::Triggers: // --triggers
        // complementar with NoTriggers
        setOptionPrivate(MySQLDumpOption::NoTriggers, !enabled);
        break;

    case MySQLDumpOption::NoTriggers: // --skip-triggers
        // complementar with Triggers
        setOptionPrivate(MySQLDumpOption::Triggers, !enabled);
        break;
    // ----------------------------->


    case MySQLDumpOption::AddDropTable:
        // --opt has --add-drop-table
        setOptGroupExpanded(enabled, MySQLDumpOption::AddDropTable);
        break;

    default:
        break;
    }
}

void ExportDatabaseForm::setOptGroupExpanded(bool enabled,
                                             MySQLDumpOption exclude)
{
    //  --opt
    // This option, enabled by default, is shorthand for the combination of --add-drop-table --add-
    // locks --create-options --disable-keys --extended-insert --lock-tables --quick
    // --set-charset. It gives a fast dump operation and produces a dump file that can be reloaded into
    // a MySQL server quickly.

    // Because the --opt option is enabled by default, you only specify its converse, the --skip-opt
    // to turn off several default settings.


    setOption(MySQLDumpOption::NoOptGroup,     !enabled);

    if (exclude != MySQLDumpOption::AddDropTable) {
        setOption(MySQLDumpOption::AddDropTable,   !enabled);
    }
    setOption(MySQLDumpOption::AddLocks,       !enabled);
    setOption(MySQLDumpOption::CreateOptions,  !enabled);
    setOption(MySQLDumpOption::DisableKeys,    !enabled);
    setOption(MySQLDumpOption::ExtendedInsert, !enabled);
    setOption(MySQLDumpOption::LockTables,     !enabled);
    setOption(MySQLDumpOption::Quick,          !enabled);
    setOption(MySQLDumpOption::SetCharset,     !enabled);

}

void ExportDatabaseForm::resetOptionsToDefault()
{
    _options = 0;

    setOption(MySQLDumpOption::AllDatabases, true);
    setOption(MySQLDumpOption::Databases, false);
    //setOption(MySQLDumpOption::AddDropDatabase, false);
    setOption(MySQLDumpOption::AddDropTable, true);
    //setOption(MySQLDumpOption::AddDropTrigger, false);
    setOption(MySQLDumpOption::CreateDatabase, true);
    setOption(MySQLDumpOption::CreateTable, true);
    //setOption(MySQLDumpOption::NoCreateDatabase, false);
    //setOption(MySQLDumpOption::ReplaceNotInsert, false);
    setOption(MySQLDumpOption::Events, true);
    //setOption(MySQLDumpOption::NoData, false);
    setOption(MySQLDumpOption::Routines, true);
    setOption(MySQLDumpOption::Triggers, true);
}

} // namespace forms
} // namespace models
} // namespace meow

