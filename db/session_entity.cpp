#include "session_entity.h"
#include "connections_manager.h"

namespace meow {
namespace db {

SessionEntity::SessionEntity(ConnectionPtr connection, ConnectionsManager * parent)
    :Entity(parent),
     _connection(connection)
{

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

int SessionEntity::childCount() const // override
{
    return 1; // just to test
}

Entity * SessionEntity::child(int row) const // override
{
    Q_UNUSED(row)
    return new Entity(const_cast<SessionEntity *>(this)); // just to test
}

} // namespace db
} // namespace meow
