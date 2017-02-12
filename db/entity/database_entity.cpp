#include "database_entity.h"
#include "session_entity.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace db {

DataBaseEntity::DataBaseEntity(const QString & dbName, SessionEntity * parent)
    :Entity(parent),
     _dbName(dbName),
     _entities(),
     _entitiesWereInit(false)
{

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
    return QIcon(":/icons/database.png");
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

void DataBaseEntity::initEntitiesIfNeed()
{
    if (_entitiesWereInit == false) {

        _entities = connection()->getDbEntities(_dbName);

        qDebug() << "cnt: " << _entities->list()->count();

        _entitiesWereInit = true;
    }
}

} // namespace db
} // namespace meow
