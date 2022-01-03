#include "select_db_object_form.h"
#include "db/connection.h"

namespace meow {
namespace ui {
namespace presenters {

SelectDBObjectForm::SelectDBObjectForm(meow::db::SessionEntity * session)
    : QObject()
    , _session(session)
{

}

void SelectDBObjectForm::setTreeSelection(const Object & object)
{
    QString prevDBName = _currentSelection.databaseName;
    _currentSelection = object;
    if (_currentSelection.databaseName.isEmpty()) {
        _currentSelection.databaseName = "%";
    } else {
        _currentSelection.databaseName = _session->connection()->escapeString(
                    _currentSelection.databaseName,
                    true, // escape _ and % as they mean wildcard
                    false);
    }
    if (prevDBName != _currentSelection.databaseName) {
        emit databasenameChanged(_currentSelection.databaseName);
    }
}

void SelectDBObjectForm::setDatabaseWildcard(const QString & databaseName)
{
    QString prevDBName = _currentSelection.databaseName;
    presenters::SelectDBObjectForm::Object dbObject(
        meow::db::Entity::Type::Database,
        databaseName
    );
    _currentSelection = dbObject;
    if (prevDBName != _currentSelection.databaseName) {
        emit databasenameChanged(_currentSelection.databaseName);
    }
}

} // namespace presenters
} // namespace ui
} // namespace meow
