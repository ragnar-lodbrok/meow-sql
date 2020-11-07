#include "edit_database_form.h"
#include "db/connection.h"
#include "db/entity/session_entity.h"
#include "db/entity/database_entity.h"

namespace meow {
namespace models {
namespace forms {

EditDatabaseForm::EditDatabaseForm(
        db::SessionEntity * session,
        meow::db::DataBaseEntity * database,
        bool createMode)
    : _session(session)
    , _database(database)
    , _createMode(createMode)
    , _dropOldDatabase(false)
{
    if (_createMode) {
        _collation = _session->connection()->serverPrefferedCollation();
    } else {
        Q_ASSERT(_database != nullptr);
        _name = _database->name();
        //_collation = _session->collation() TODO
    }

}

QString EditDatabaseForm::name() const
{
    return _name;
}

void EditDatabaseForm::setName(const QString & name)
{
    _name = name;
}

QString EditDatabaseForm::originalName() const
{
    if (_database) {
        return _database->name();
    }
    return QString();
}

const QStringList EditDatabaseForm::collations() const
{
    return _session->connection()->collationList();
}

QString EditDatabaseForm::collation() const
{
    return _collation;
}

void EditDatabaseForm::setCollation(const QString & collation)
{
    _collation = collation;
}

void EditDatabaseForm::save()
{
    _dropOldDatabase = false;

    // TODO: pass collation
    if (_createMode) {
        _session->createDatabase(_name/*, _collation*/);
    } else {
        _dropOldDatabase = _session->editDatabase(_database,
                                             _name/*, _collation*/);
    }
}

bool EditDatabaseForm::databaseWithEnteredNameExists(bool refresh) const
{
    QStringList allDatabases = _session->connection()->allDatabases(refresh);
    return allDatabases.contains(_name);
}

bool EditDatabaseForm::canSave() const
{
    return !_name.isEmpty();
}


} // namespace forms
} // namespace models
} // namespace meow

