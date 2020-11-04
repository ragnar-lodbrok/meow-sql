#include "mysql_database_editor.h"
#include "db/connection.h"
#include "db/entity/database_entity.h"
#include <QDebug>

namespace meow {
namespace db {

MySQLDataBaseEditor::MySQLDataBaseEditor(Connection * connection)
    :DataBaseEditor(connection)
{

}

void MySQLDataBaseEditor::create(const QString & name,
                            const QString & collation)
{
    QString SQL = QString("CREATE DATABASE %1")
            .arg(_connection->quoteIdentifier(name));

    if (!collation.isEmpty()) {
        SQL += QString(" COLLATE %1")
                .arg(_connection->escapeString(collation));
    }

    _connection->query(SQL);
}

bool MySQLDataBaseEditor::edit(DataBaseEntity * database,
                          const QString & newName,
                          const QString & newCollation)
{
    bool nameChanged = database->name() != newName;
    bool collationChanged = database->defaultCollation() != newCollation;

    if (!nameChanged && !collationChanged) {
        return false;
    }

    if (collationChanged && !nameChanged) {
        QString SQL = QString("ALTER DATABASE %1 COLLATE %2")
                .arg(_connection->quoteIdentifier(database->name()))
                .arg(_connection->escapeString(newCollation));
        _connection->query(SQL);
        return true;
    }

    for (int i=0; i < database->childCount(); ++i) {
        Entity * childEntity = database->child(i);
        if (childEntity->type() != db::Entity::Type::Table
            && childEntity->type() != db::Entity::Type::View) {

            throw db::Exception(
                QObject::tr(
                    "Database '%1' contains stored routine(s),"
                    " which cannot be moved.")
                .arg(database->name())
            );
        }
    }

    bool newDatabaseExists =  database->connection()
            ->allDatabases().contains(newName);

    // create db entity to fetch subentities
    DataBaseEntity * newDatabase = new DataBaseEntity(newName,
                                                      database->session());
    std::unique_ptr<DataBaseEntity> newDbPtr(newDatabase);

    for (int i=0; newDatabaseExists && i < database->childCount(); ++i) {
        Entity * oldEntity = database->child(i);
        if (newDatabase->hasChild(oldEntity->name(), oldEntity->type())) {
            throw db::Exception(
                QObject::tr(
                    "Database '%1' exists and has"
                    " objects with same names as in '%2'")
                .arg(newDatabase->name())
                .arg(database->name())
            );
        }
    }

    if (newDatabaseExists == false) {
        this->create(newName, newCollation);
    }

    QStringList SQLRenames;
    for (int i=0; i < database->childCount(); ++i) {
        Entity * oldEntity = database->child(i);
        SQLRenames << QString("%1.%2 TO %3.%2")
                .arg(_connection->quoteIdentifier(database->name()))
                .arg(_connection->quoteIdentifier(oldEntity->name()))
                .arg(_connection->quoteIdentifier(newDatabase->name()));
    }

    if (!SQLRenames.isEmpty()) {
        QString SQL = "RENAME TABLE " + SQLRenames.join(", ");
        _connection->query(SQL);
    }

    // DROP Outside rename here only
    /*QString dropSQL = "DROP DATABASE "
            + _connection->quoteIdentifier(database->name());
    _connection->query(dropSQL);*/

    return true;
}

} // namespace db
} // namespace meow
