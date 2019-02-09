#include "entity_filter.h"
#include "db/connection.h"
#include "db/entity/table_entity.h"
#include "db/entity/entity_list_for_database.h"

namespace meow {
namespace db {

EntityFilter::EntityFilter(Connection * connection)
    :_connection(connection)
{

}

EntityFilter::~EntityFilter()
{

}

TableEntity * EntityFilter::tableByName(
        const QString & databaseName,
        const QString & tableName) const
{
    QList<Entity *> * entityList
        = _connection->getDbEntities(databaseName)->list();

    for (Entity * entity : *entityList) {
        if (entity->type() == Entity::Type::Table) {
            TableEntity * table = static_cast<TableEntity *>(entity);
            if (table->name() == tableName) {
                return table;
            }
        }
    }

    return nullptr;
}

} // namespace db
} // namespace meow
