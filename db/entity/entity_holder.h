#ifndef DB_ENTITY_HOLDER_H
#define DB_ENTITY_HOLDER_H

#include "db/entity/entity.h"

namespace meow {
namespace db {

// Intent: stores Entity and provides its changes/properties
class EntityHolder
{
public:
    EntityHolder();

    bool setCurrentEntity(Entity * currentEntity);
    Entity * currentEntity() const { return _currentEntity; }

    bool connectionChanged() const;
    bool databaseChanged() const;
    bool hasDatabase() const;

private:
    Entity * _prevEntity;
    Entity * _currentEntity;
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_HOLDER_H
