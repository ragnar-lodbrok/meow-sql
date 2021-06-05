#include "mysql_entity_filter.h"
#include "db/mysql/mysql_connection.h"
#include "db/entity/table_entity.h"

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

    QList<EntityPtr> entityList = _connection->getDbEntities(dbName);

    for (const EntityPtr & entity : entityList) {
        if (entity->type() == Entity::Type::Table) {
            TableEntityPtr table = std::static_pointer_cast<TableEntity>(entity);
            if (table->engineStr().toLower() == "innodb") {
                tables << table.get();
            }
        }
    }

    return tables;
}

} // namespace db
} // namespace meow
