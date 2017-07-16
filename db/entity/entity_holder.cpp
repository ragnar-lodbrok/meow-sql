#include "entity_holder.h"

namespace meow {
namespace db {

EntityHolder::EntityHolder()
    : _prevEntity(nullptr),
      _currentEntity(nullptr)
{

}


bool EntityHolder::setCurrentEntity(db::Entity * currentEntity)
{
    _prevEntity = _currentEntity;
    _currentEntity = currentEntity;
    return _prevEntity != _currentEntity;
}

bool EntityHolder::connectionChanged() const
{
    if (_prevEntity == _currentEntity) {
        return false;
    }

    if (_prevEntity == nullptr || _currentEntity == nullptr) {
        return true;
    }

    return _currentEntity->connection() != _prevEntity->connection();
}

bool EntityHolder::databaseChanged() const
{
    if (_prevEntity == _currentEntity) {
        return false;
    }

    if (_prevEntity == nullptr || _currentEntity == nullptr) {
        return true;
    }

    db::Entity * prevDatabaseEntity =
        db::findParentEntityOfType(_prevEntity, db::Entity::Type::Database);

    db::Entity * curDatabaseEntity =
        db::findParentEntityOfType(_currentEntity, db::Entity::Type::Database);

    return prevDatabaseEntity != curDatabaseEntity;
}

bool EntityHolder::hasDatabase() const
{
    if (_currentEntity == nullptr) {
        return false;
    }

    return db::findParentEntityOfType(
        _currentEntity, db::Entity::Type::Database) != nullptr;
}

} // namespace db
} // namespace meow
