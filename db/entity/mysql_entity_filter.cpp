#include "mysql_entity_filter.h"
#include "db/mysql_connection.h"
#include "db/entity/table_entity.h"
#include "db/entity/entity_list_for_database.h"

namespace meow {
namespace db {

MySQLEntityFilter::MySQLEntityFilter(MySQLConnection * connection)
    :EntityFilter(connection)
{

}

QList<TableEntity *> MySQLEntityFilter::tablesWithForeignKeySupport(
    const QString &dbName) const
{
    QList<TableEntity *> tables;

    QList<Entity *> * entityList = _connection->getDbEntities(dbName)->list();

    for (Entity * entity : *entityList) {
        if (entity->type() == Entity::Type::Table) {
            TableEntity * table = static_cast<TableEntity *>(entity);
            if (table->engineStr().toLower() == "innodb") {
                tables << table;
            }
        }
    }

    return tables;
}

} // namespace db
} // namespace meow
