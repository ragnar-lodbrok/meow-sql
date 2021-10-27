#include "entity_holder.h"
#include <QDebug>

namespace meow {
namespace db {

EntityHolder::EntityHolder()
    : _currentEntity(nullptr)
    , _connectionChanged(false)
    , _databaseChanged(false)
{

}


bool EntityHolder::setCurrentEntity(const db::EntityPtr & currentEntity)
{
    // TODO: own currentEntity->session() object to avoid dangling session ptr?

    auto prevEntity = _currentEntity;
    _currentEntity = currentEntity;

    _connectionChanged = connectionChanged(prevEntity, _currentEntity);
    _databaseChanged = databaseChanged(prevEntity, _currentEntity);

    return prevEntity != _currentEntity;
}

bool EntityHolder::connectionChanged(const db::EntityPtr & prevEntity,
                                     const db::EntityPtr & currentEntity) const
{
    if (prevEntity == currentEntity) {
        return false;
    }

    if (prevEntity == nullptr || currentEntity == nullptr) {
        return true;
    }

    return currentEntity->connection() != prevEntity->connection();
}

bool EntityHolder::databaseChanged(const db::EntityPtr & prevEntity,
                                   const db::EntityPtr & currentEntity) const
{
    if (prevEntity == currentEntity) {
        return false;
    }

    if (prevEntity == nullptr || currentEntity == nullptr) {
        return true;
    }

    db::Entity * prevDatabaseEntity =
        db::findParentEntityOfType(prevEntity.get(),
                                   db::Entity::Type::Database);

    db::Entity * curDatabaseEntity =
        db::findParentEntityOfType(currentEntity.get(),
                                   db::Entity::Type::Database);

    return prevDatabaseEntity != curDatabaseEntity;
}

bool EntityHolder::hasDatabase() const
{
    if (_currentEntity == nullptr) {
        return false;
    }

    return db::findParentEntityOfType(
        _currentEntity.get(), db::Entity::Type::Database) != nullptr;
}

} // namespace db
} // namespace meow
