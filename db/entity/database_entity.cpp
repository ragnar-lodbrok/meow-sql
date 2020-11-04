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
     _entities(nullptr),
     _entitiesWereInit(false)
{

}

DataBaseEntity::~DataBaseEntity()
{
    qDebug() << "~DataBaseEntity" << _dbName;
    clearChildren();
}

QString DataBaseEntity::name() const // override
{
    return _dbName;
}

Connection * DataBaseEntity::connection() const // override
{
    return session()->connection();
}

int DataBaseEntity::row() const // override
{
    return session()->indexOf(const_cast<DataBaseEntity *>(this));
}

QVariant DataBaseEntity::icon() const // override
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

int DataBaseEntity::childCount() // override
{
    initEntitiesIfNeed();
    return _entities->list()->size();
}

Entity * DataBaseEntity::child(int row) // override
{
    initEntitiesIfNeed();
    return _entities->list()->value(row); // null if out of bounds
}

db::ulonglong DataBaseEntity::dataSize() const // override
{
    db::ulonglong sum = 0;

    if (_entities) {
        for (auto entity : *(_entities->list())) {
            sum += entity->dataSize();
        }
    }

    return sum;
}

void DataBaseEntity::initEntitiesIfNeed()
{
    if (_entitiesWereInit == false) {

        _entities = connection()->getDbEntities(_dbName);

        for (auto entity : *(_entities->list())) {
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
    return _entities->list()->indexOf(entity);
}

bool DataBaseEntity::hasChild(const QString & name, const Type type)
{
    initEntitiesIfNeed();
    return _entities->hasEntity(name, type);
}

void DataBaseEntity::appendEntity(EntityInDatabase * entity)
{
    initEntitiesIfNeed();
    if (_entities) {
        _entities->list()->append(entity);
    }
}

bool DataBaseEntity::removeEntity(EntityInDatabase * entity)
{
    initEntitiesIfNeed(); // TODO: rm?
    if (_entities) {
        return _entities->list()->removeOne(entity);
    }
    return false;
}

} // namespace db
} // namespace meow
