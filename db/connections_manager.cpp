#include "connections_manager.h"
#include "connection.h"
#include "user_query/user_query.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include <QDebug>

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

    connect(newSession,
            &meow::db::SessionEntity::entityEdited,
            this,
            &meow::db::ConnectionsManager::onEntityEdited);

    connect(newSession,
            &meow::db::SessionEntity::entityInserted,
            this,
            &meow::db::ConnectionsManager::onEntityInserted);

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

void ConnectionsManager::createEntity(Entity::Type type)
{
    Entity * currentEntity = _activeEntity.currentEntity();
    if (!currentEntity) return;

    DataBaseEntity * databaseEntity =
        static_cast<DataBaseEntity *>(
            db::findParentEntityOfType(currentEntity, Entity::Type::Database)
    );
    if (!databaseEntity) return;

    if (type == Entity::Type::Table) {
        TableEntity * table = new TableEntity("", databaseEntity);
        table->setIsNew(true);
        _activeEntity.setCurrentEntity(nullptr);
        emit creatingNewEntity(table);
        //emit activeEntityChanged(table);
    }
}

void ConnectionsManager::setActiveEntity(Entity * activeEntity)
{
    bool changed = _activeEntity.setCurrentEntity(activeEntity);
    if (changed) {
        // TODO: Session changed

        Connection * connection = activeConnection();

        if (connection) {
            if (_activeEntity.databaseChanged()) {
                QString dbName = databaseName(activeEntity);
                connection->setDatabase(dbName);
            }

            if (activeEntity->type() == Entity::Type::Table) {
                TableEntity * table = static_cast<TableEntity *>(activeEntity);
                connection->parseTableStructure(table);
            }
        }
        emit activeEntityChanged(activeEntity);
    }
}

void ConnectionsManager::onEntityEdited(Entity * entity)
{
    emit entityEdited(entity);
}

void ConnectionsManager::onEntityInserted(Entity * entity)
{
    emit entityInserted(entity);
    setActiveEntity(entity);
}

} // namespace db
} // namespace meow
