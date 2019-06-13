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

class ExportDatabaseForm : public QObject
{
    Q_OBJECT

public:
    ExportDatabaseForm(meow::db::SessionEntity * session);

    const QStringList databases() const;

    void setDatabase(const QString & db) { _database = db; }
    const QString database() const {
        return _allDatabases ? QString() : _database;
    }

    void setAllDatabases(bool all) { _allDatabases = all; }
    bool allDatabases() const { return _allDatabases; }

    void setFilename(const QString & name) { _filename = name; }
    const QString & filename() const { return _filename; }


    meow::db::SessionEntity * session() const { return _session; }

    void startExport();
    void cancelExport();

    Q_SIGNAL void finished(bool success);
    Q_SIGNAL void progressMessage(const QString & str);

private:

    QString generateFilename() const;

    meow::db::SessionEntity * const _session;
    std::unique_ptr<meow::utils::exporting::MySQLDumpConsole> _dumper;

    QString _database;
    bool _allDatabases;

    QString _filename;
};

} // namespace forms
} // namespace models
} // namespace meow


#endif // MODELS_EXPORT_DATABASE_FORM_H
