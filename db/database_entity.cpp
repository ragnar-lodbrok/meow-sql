#include "database_entity.h"
#include "session_entity.h"
#include <QIcon>

namespace meow {
namespace db {

DataBaseEntity::DataBaseEntity(const QString & dbName, SessionEntity * parent)
    :Entity(parent),
     _dbName(dbName)
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

} // namespace db
} // namespace meow
