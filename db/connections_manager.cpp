#include "connections_manager.h"
#include "connection.h"
#include "user_query/user_query.h"

namespace meow {
namespace db {

ConnectionsManager::ConnectionsManager()
    :QObject(),
     Entity(),
     _connections(),
     _activeEntity(),
     _userQueries()
{

}

ConnectionsManager::~ConnectionsManager()
{
    qDeleteAll(_connections);
    qDeleteAll(_userQueries);
}

ConnectionPtr ConnectionsManager::openDBConnection(db::ConnectionParameters & params)
{
    ConnectionPtr connection = params.createConnection();

    connection->setActive(true);

    SessionEntity * newSession = new SessionEntity(connection, this);

    _connections.push_back(newSession);
    _activeSession = newSession;

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

Connection * ConnectionsManager::activeConnection() const
{
    if (_activeSession) {
        return _activeSession->connection();
    }
    return nullptr;
}

UserQuery * ConnectionsManager::userQueryAt(size_t index)
{
    if (index + 1 > _userQueries.size()) {
        _userQueries.resize(index + 1, nullptr);
    }
    if (!_userQueries[index]) {
        _userQueries[index] = new UserQuery(this);
    }
    return _userQueries[index];
}

void ConnectionsManager::setActiveEntity(Entity * activeEntity)
{
    bool changed = _activeEntity.setCurrentEntity(activeEntity);
    if (changed) {
        // TODO: Session changed

        if (_activeEntity.databaseChanged()) {
            Connection * connection = activeConnection();
            if (connection) {
                QString dbName = databaseName(activeEntity);
                connection->setDatabase(dbName);
            }
        }

        emit activeEntityChanged(activeEntity);
    }
}

} // namespace db
} // namespace meow
