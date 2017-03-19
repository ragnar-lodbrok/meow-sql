#include "connections_manager.h"
#include "connection.h"

namespace meow {
namespace db {

ConnectionsManager::ConnectionsManager()
    :QObject(),
     Entity(),
     _connections(),
     _activeEntity(nullptr)
{

}

ConnectionsManager::~ConnectionsManager()
{
    qDeleteAll(_connections);
}

ConnectionPtr ConnectionsManager::openDBConnection(db::ConnectionParameters & params)
{
    ConnectionPtr connection = params.createConnection();

    connection->setActive(true);

    SessionEntity * newSession = new SessionEntity(connection, this);

    _connections.push_back(newSession);

    emit connectionOpened(newSession);

    return connection;
}

int ConnectionsManager::childCount() // override
{
    return _connections.size();
}

SessionEntity * ConnectionsManager::child(int row) // override
{
    return _connections.value(row); // returns null if out of bounds
}

int ConnectionsManager::indexOf(SessionEntity * session) const
{
    return _connections.indexOf(session);
}

void ConnectionsManager::setActiveEntity(Entity * activeEntity)
{
    bool changed = _activeEntity != activeEntity;
    _activeEntity = activeEntity;
    if (changed) {
        emit activeEntityChanged(_activeEntity);
    }
}

} // namespace db
} // namespace meow
