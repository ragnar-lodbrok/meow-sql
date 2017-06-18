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

} // namespace db
} // namespace meow

