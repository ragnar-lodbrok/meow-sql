#include "edit_database_form.h"

#include "db/connection.h"

namespace meow {
namespace models {
namespace forms {

EditDatabaseForm::EditDatabaseForm(db::Connection * connection)
    :_connection(connection)
{
    _collation = _connection->serverPrefferedCollation();
}

QString EditDatabaseForm::name() const
{
    return _name;
}

void EditDatabaseForm::setName(const QString & name)
{
    _name = name;
}

const QStringList EditDatabaseForm::collations() const
{
    return _connection->collationList();
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
    _connection->createDatabase(_name, _collation);
}

bool EditDatabaseForm::canSave() const
{
    return !_name.isEmpty();
}


} // namespace forms
} // namespace models
} // namespace meow

