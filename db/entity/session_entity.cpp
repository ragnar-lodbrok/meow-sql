#include "session_entity.h"
#include "db/connections_manager.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

SessionEntity::SessionEntity(ConnectionPtr connection, ConnectionsManager * parent)
    :Entity(parent),
     _connection(connection),
     _databases(),
     _databasesWereInit(false)
{

}

SessionEntity::~SessionEntity()
{
    qDeleteAll(_databases);
}

QString SessionEntity::name() const // override
{
    return connection()->connectionParams()->sessionName();
}

Connection * SessionEntity::connection() const // override
{
    return _connection.get();
}

ConnectionsManager * SessionEntity::connectionsManager() const
{
    return static_cast<ConnectionsManager *>(parent());
}

int SessionEntity::row() const // override
{
    return connectionsManager()->indexOf(const_cast<SessionEntity *>(this));
}

int SessionEntity::childCount() // override
{
    initDatabasesListIfNeed();
    return _databases.size();
}

Entity * SessionEntity::child(int row) // override
{
    initDatabasesListIfNeed();
    return _databases.value(row); // null if out of bounds
}

QVariant SessionEntity::icon() const // override
{
    return QIcon(":/icons/server-mysql.png");
}

int SessionEntity::indexOf(DataBaseEntity * session) const
{
    return _databases.indexOf(session);
}

void SessionEntity::initDatabasesListIfNeed()
{
    if (_databasesWereInit == false) {

        QStringList databaseNames = connection()->allDatabases();

        foreach (const QString &dbName, databaseNames) {
            DataBaseEntity * dbEntity = new DataBaseEntity(dbName, const_cast<SessionEntity *>(this));
            _databases.push_back(dbEntity);
        }

        _databasesWereInit = true;
    }
}

} // namespace db
} // namespace meow
