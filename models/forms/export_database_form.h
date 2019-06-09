#ifndef MODELS_EXPORT_DATABASE_FORM_H
#define MODELS_EXPORT_DATABASE_FORM_H

#include <QStringList>

namespace meow {

namespace db {
   class SessionEntity;
}

namespace models {
namespace forms {

class ExportDatabaseForm
{
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

private:

    QString generateFilename() const;

    meow::db::SessionEntity * _session;

    QString _database;
    bool _allDatabases;

    QString _filename;
};

} // namespace forms
} // namespace models
} // namespace meow


#endif // MODELS_EXPORT_DATABASE_FORM_H
