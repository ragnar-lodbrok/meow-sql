#ifndef MODELS_EXPORT_DATABASE_FORM_H
#define MODELS_EXPORT_DATABASE_FORM_H

#include <memory>
#include "utils/exporting/mysql_dump_console.h"

namespace meow {

namespace db {
   class SessionEntity;
}

namespace utils {
namespace exporting {
    class MySQLDumpConsole;
}
}

namespace models {
namespace forms {


enum class MySQLDumpOption
{
    AllDatabases       = 1, //  --all-databases
    Databases          = (1 << 2), //  --databases
    //SingleDatabase   = (1 << 3), // no *-databases key TODO
    AddDropDatabase    = (1 << 4), //  --add-drop-database
    AddDropTable       = (1 << 5), //  --add-drop-table
    AddDropTrigger     = (1 << 6), //  --add-drop-trigger
    CreateDatabase     = (1 << 7), // on when --all-databases or --databases
    CreateTable        = (1 << 8),
    //ReplaceNotInsert = (1 << 9), // --replace // TODO
    Events             = (1 << 10), // --events
    //NoData           = (1 << 11), // --no-data // TODO
    Routines           = (1 << 12), // --routines
    Triggers           = (1 << 13), // --triggers (enabled by default)
    NoColumnStatistics = (1 << 14), // --column-statistics=0 (v8 and higher)

    // internal options:

    NoCreateDatabase   = (1 << 15), // --no-create-db
    NoTriggers         = (1 << 16), // --skip-triggers
    NoCreateTable      = (1 << 17), //  --no-create-info
    //OptGroup         = (1 << 18), // --opt
    NoOptGroup         = (1 << 19), // --skip-opt
    AddLocks           = (1 << 20), // --add-locks
    CreateOptions      = (1 << 21), // --create-options
    DisableKeys        = (1 << 22), // --disable-keys
    ExtendedInsert     = (1 << 23), // --extended-insert
    LockTables         = (1 << 24), // --lock-tables
    Quick              = (1 << 25), // --quick
    SetCharset         = (1 << 26), // --set-charset

};

// TODO: the form should not contain MySQL-specific logic?
class ExportDatabaseForm : public QObject
{
    Q_OBJECT

public:
    ExportDatabaseForm(meow::db::SessionEntity * session);

    const QStringList databases() const;

    void setDatabase(const QString & db);
    const QString database() const {
        return allDatabases() ? QString() : _database;
    }

    void setAllDatabases(bool all);
    bool allDatabases() const {
        return isOptionEnabled(MySQLDumpOption::AllDatabases);
    }

    void setFilename(const QString & name);
    const QString & filename() const { return _filename; }


    meow::db::SessionEntity * session() const { return _session; }

    void startExport();
    bool cancelExport();

    bool isOptionEnabled(MySQLDumpOption opt) const {
        return (_options & static_cast<uint32_t>(opt))
                == static_cast<uint32_t>(opt);
    }

    void setOption(MySQLDumpOption opt, bool enabled);

    QString currentCommand() const;

    Q_SIGNAL void finished(bool success);
    Q_SIGNAL void progressMessage(const QString & str);

    Q_SIGNAL void optionsChanged();

private:

    QString generateFilename() const;
    void resetOptionsToDefault();

    void setOptGroupExpanded(bool enabled, MySQLDumpOption exclude);

    void setOptionPrivate(MySQLDumpOption opt, bool enabled);

    meow::db::SessionEntity * const _session;
    std::unique_ptr<meow::utils::exporting::MySQLDumpConsole> _dumper;

    QString _database;

    QString _filename;
    bool _filenameChangedByUser;

    uint32_t _options;
};

} // namespace forms
} // namespace models
} // namespace meow


#endif // MODELS_EXPORT_DATABASE_FORM_H
