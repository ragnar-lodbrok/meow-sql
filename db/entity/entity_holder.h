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

    bool setCurrentEntity(const db::EntityPtr & currentEntity);
    Entity * currentEntity() const { return _currentEntity.get(); }

    bool connectionChanged() const {
        return _connectionChanged;
    }
    bool databaseChanged() const {
        return _databaseChanged;
    }
    bool hasDatabase() const;

private:

    bool connectionChanged(const db::EntityPtr & prevEntity,
                           const db::EntityPtr & currentEntity) const;
    bool databaseChanged(const db::EntityPtr & prevEntity,
                         const db::EntityPtr & currentEntity) const;

    EntityPtr _currentEntity;

    bool _connectionChanged;
    bool _databaseChanged;
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_HOLDER_H
