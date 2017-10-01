#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Entity * parent /* = nullptr */):
    _parent(parent),
    _wasSelected(false),
    _createCodeCached(false, QString())
{

}

Connection * Entity::connection() const
{
    if (_parent && _parent->type() != Type::None) {
        return _parent->connection();
    }
    return nullptr;
}

QString Entity::createCode(bool refresh)
{
    if (_createCodeCached.first == false || refresh) {
        if (connection()) {
            _createCodeCached.second = connection()->getCreateCode(this);
            _createCodeCached.first = true;
        }
    }

    return _createCodeCached.second;
}

// --------------------------------------------------------------

Entity * findParentEntityOfType(const Entity * entity, Entity::Type type)
{
    if (type == Entity::Type::None) {
        return nullptr;
    } else if (entity->type() == type) {
        return const_cast<Entity *>(entity);
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

QString databaseName(const Entity *entity)
{
    Entity * databaseEntity =
        findParentEntityOfType(entity, db::Entity::Type::Database);

    if (databaseEntity) {
        return databaseEntity->name();
    }

    return QString();
}

QString quotedName(const Entity * entity)
{
    Connection * conn = entity->connection();
    if (conn) {
        return conn->quoteIdentifier(entity->name());
    }
    return entity->name();
}

QString quotedDatabaseName(const Entity * entity)
{
    QString database = databaseName(entity);
    Connection * conn = entity->connection();
    if (conn && database.length()) {
        return conn->quoteIdentifier(database);
    }
    return database;
}

QString quotedFullName(const Entity * entity)
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

