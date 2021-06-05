#include "database_entity.h"
#include "session_entity.h"
#include "table_entity.h"
#include "db/connection.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace db {

DataBaseEntity::DataBaseEntity(const QString & dbName, SessionEntity * parent)
    :Entity(parent),
     _dbName(dbName),
     _entitiesWereInit(false)
{

}

DataBaseEntity::~DataBaseEntity()
{
    clearChildren();
}

QString DataBaseEntity::name() const
{
    return _dbName;
}

Connection * DataBaseEntity::connection() const
{
    return session()->connection();
}

int DataBaseEntity::row() const
{
    return session()->indexOf(const_cast<DataBaseEntity *>(this));
}

QVariant DataBaseEntity::icon() const
{
    static const QIcon icon = QIcon(":/icons/database.png");
    static const QIcon activeIcon = QIcon(":/icons/database_active.png");

    bool isActive = session()->activeDatabase() == this;

    return isActive ? activeIcon : icon;
}

SessionEntity * DataBaseEntity::session() const
{
    return static_cast<SessionEntity *>(parent());
}

int DataBaseEntity::childCount()
{
    initEntitiesIfNeed();
    return _entities.size();
}

Entity * DataBaseEntity::child(int row)
{
    initEntitiesIfNeed();
    return _entities.value(row).get();
}

db::ulonglong DataBaseEntity::dataSize() const
{
    db::ulonglong sum = 0;
    for (auto entity : _entities) {
        sum += entity->dataSize();
    }

    return sum;
}

void DataBaseEntity::initEntitiesIfNeed()
{
    if (_entitiesWereInit == false) {

        _entities = connection()->getDbEntities(_dbName);

        for (const auto & entity : _entities) {
            entity->setParent(this);
        }

        _entitiesWereInit = true;
    }
}

bool DataBaseEntity::childrenFetched() const
{
    return _entitiesWereInit;
}

void DataBaseEntity::clearChildren()
{
    connection()->deleteAllCachedEntitiesInDatabase(_dbName);
    _entitiesWereInit = false;
}

int DataBaseEntity::indexOf(Entity * entity)
{
    initEntitiesIfNeed();
    return _entities.indexOf(entity->retain());
}

bool DataBaseEntity::hasChild(const QString & name, const Type type)
{
    initEntitiesIfNeed();
    return hasEntity(name, type);
}

void DataBaseEntity::appendEntity(EntityInDatabase * entity)
{
    initEntitiesIfNeed();
    _entities.append(entity->retain());
}

bool DataBaseEntity::removeEntity(EntityInDatabase * entity)
{
    initEntitiesIfNeed(); // TODO: rm?
    return _entities.removeOne(entity->retain());
}

bool DataBaseEntity::hasEntity(const QString & name,
                               const Entity::Type type) const
{

    for (const auto & entity : _entities) {
        if (entity->type() == type && entity->name() == name) {
            return true;
        }
    }
    return false;
}

} // namespace db
} // namespace meow
