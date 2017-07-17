#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Entity * parent /* = nullptr */):
    _parent(parent),
    _wasSelected(false)
{

}

Connection * Entity::connection() const
{
    if (_parent && _parent->type() != Type::None) {
        return _parent->connection();
    }
    return nullptr;
}

// --------------------------------------------------------------

Entity * findParentEntityOfType(Entity * entity, Entity::Type type)
{
    if (type == Entity::Type::None) {
        return nullptr;
    } else if (entity->type() == type) {
        return entity;
    } else if (entity->parent()) {
        return findParentEntityOfType(entity->parent(), type);
    } else {
        return nullptr;
    }
}

int childCountOfType(Entity * entity, Entity::Type type)
{
    int childCountTotal = entity->childCount();
    int childCountOfType = 0;
    for (int i=0; i<childCountTotal; ++i) {
        Entity * child = entity->child(i);
        if (child->type() == type) {
            ++childCountOfType;
        }
    }
    return childCountOfType;
}

QString databaseName(Entity * entity)
{
    Entity * databaseEntity =
        findParentEntityOfType(entity, db::Entity::Type::Database);

    if (databaseEntity) {
        return databaseEntity->name();
    }

    return QString();
}

QString quotedName(Entity * entity)
{
    Connection * conn = entity->connection();
    if (conn) {
        return conn->quoteIdentifier(entity->name());
    }
    return entity->name();
}

QString quotedDatabaseName(Entity * entity)
{
    QString database = databaseName(entity);
    Connection * conn = entity->connection();
    if (conn && database.length()) {
        return conn->quoteIdentifier(database);
    }
    return database;
}

QString quotedFullName(Entity * entity)
{
    if (entity->type() == db::Entity::Type::Session ||
        entity->type() == db::Entity::Type::Database) {
        return quotedName(entity);
    }

    QString qName = quotedName(entity);
    QString qDatabaseName = quotedDatabaseName(entity);

    if (qName.length() && qDatabaseName.length()) {
        return qDatabaseName + "." + qName;
    } else {
        return qName;
    }
}

} // namespace db
} // namespace meow

