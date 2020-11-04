#ifndef DB_ENTITY_LIST_FOR_DATABASE_H
#define DB_ENTITY_LIST_FOR_DATABASE_H

#include <QList>
#include "entity.h"

namespace meow {
namespace db {

// Intent: list of entities for a database
// TODO: rm?
class EntityListForDataBase
{
public:
    EntityListForDataBase();
    ~EntityListForDataBase();
    QList<Entity *> * list() { return &_entities; }

    bool hasEntity(const QString & name,
                   const Entity::Type type = Entity::Type::Table) const
    {

        for (const Entity * entity : _entities) {
            if (entity->type() == type && entity->name() == name) {
                return true;
            }
        }
        return false;
    }

private:
    QList<Entity *> _entities; // TODO: map?
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_LIST_FOR_DATABASE_H
