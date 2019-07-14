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
    AllDatabases = 1, //  --all-databases
    Databases, //  --databases
    //SingleDatabase, // no *-databases key TODO
    AddDropDatabase, //  --add-drop-database
    AddDropTable, //  --add-drop-table
    AddDropTrigger, //  --add-drop-trigger
    CreateDatabase, // on when --all-databases or --databases
    CreateTable,
    //ReplaceNotInsert, // --replace // TODO
    Events, // --events
    //NoData, // --no-data // TODO
    Routines, // --routines
    Triggers, // --triggers (enabled by default)


    // internal options:

    NoCreateDatabase, // --no-create-db
    NoTriggers, // --skip-triggers
    NoCreateTable, //  --no-create-info
    //OptGroup, // --opt
    NoOptGroup, // --skip-opt
    AddLocks, // --add-locks
    CreateOptions, // --create-options
    DisableKeys, // --disable-keys
    ExtendedInsert, // --extended-insert
    LockTables, // --lock-tables
    Quick, // --quick
    SetCharset, // --set-charset

};


class ExportDatabaseForm : public QObject
{
    Q_OBJECT

public:
    ExportDatabaseForm(meow::db::SessionEntity * session);

    const QStringList databases() const;

    void setDatabase(const QString & db) { _database = db; }
    const QString database() const {
        return allDatabases() ? QString() : _database;
    }

    void setAllDatabases(bool all) {
        setOption(MySQLDumpOption::AllDatabases, all);
    }
    bool allDatabases() const {
        return isOptionEnabled(MySQLDumpOption::AllDatabases);
    }

    void setFilename(const QString & name) { _filename = name; }
    const QString & filename() const { return _filename; }


    meow::db::SessionEntity * session() const { return _session; }

    void startExport();
    bool cancelExport();

    bool isOptionEnabled(MySQLDumpOption opt) const {
        return _options & static_cast<uint32_t>(opt);
    }

    void setOption(MySQLDumpOption opt, bool enabled);

    Q_SIGNAL void finished(bool success);
    Q_SIGNAL void progressMessage(const QString & str);

private:

    QString generateFilename() const;
    void resetOptionsToDefault();

    void setOptGroupExpanded(bool enabled, MySQLDumpOption exclude);

    meow::db::SessionEntity * const _session;
    std::unique_ptr<meow::utils::exporting::MySQLDumpConsole> _dumper;

    QString _database;
    bool _allDatabases;

    QString _filename;

    uint32_t _options;
};

} // namespace forms
} // namespace models
} // namespace meow


#endif // MODELS_EXPORT_DATABASE_FORM_H
