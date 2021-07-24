#include "connections_manager.h"
#include "connection.h"
#include "user_query/user_query.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/view_entity.h"
#include "db/entity/routine_entity.h"
#include "db/entity/trigger_entity.h"
#include "helpers/logger.h"
#include "db/entity/entity_factory.h"

namespace meow {
namespace db {

std::shared_ptr<ConnectionsManager> ConnectionsManager::create()
{
    return std::shared_ptr<ConnectionsManager>(new ConnectionsManager());
}

ConnectionsManager::ConnectionsManager()
    : Entity(),
     _activeEntity(),
     _activeSession(nullptr),
     _userQueries()
{
    // since Manager is a root entity - it might be a good place (?) to do:
    qRegisterMetaType<EntityPtr>("EntityPtr");
    qRegisterMetaType<DataBaseEntityPtr>("DataBaseEntityPtr");
    qRegisterMetaType<ViewEntityPtr>("ViewEntityPtr");
    qRegisterMetaType<TableEntityPtr>("TableEntityPtr");
    qRegisterMetaType<RoutineEntityPtr>("RoutineEntityPtr");
    qRegisterMetaType<TriggerEntityPtr>("TriggerEntityPtr");
    qRegisterMetaType<SessionEntityPtr>("SessionEntityPtr");

    appendNewUserQuery();
}

ConnectionsManager::~ConnectionsManager()
{
    qDeleteAll(_userQueries);
}

ConnectionPtr ConnectionsManager::openDBConnection(db::ConnectionParameters & params)
{
    ConnectionPtr connection = params.createConnection();

    connection->setActive(true);

    SessionEntityPtr newSession = EntityFactory::createSession(connection, this);

    QObject::connect(newSession.get(),
            &meow::db::SessionEntity::entityEdited,
            this,
            &meow::db::ConnectionsManager::onEntityEdited);

    QObject::connect(newSession.get(),
            &meow::db::SessionEntity::entityInserted,
            this,
            &meow::db::ConnectionsManager::onEntityInserted,
            Qt::QueuedConnection);

    QObject::connect(newSession->connection(),
            &meow::db::Connection::databaseChanged,
            this,
            &meow::db::ConnectionsManager::activeDatabaseChanged);

    _connections.push_back(newSession);

    emit connectionOpened(newSession.get());

    return connection;
}

bool ConnectionsManager::closeActiveConnection()
{
    SessionEntity * session = activeSession();
    if (session) {
        setActiveSession(nullptr);
        emit beforeConnectionClosed(session);
        _connections.removeOne(session->retain());
        return true;
    }
    return false;
}

int ConnectionsManager::childCount()
{
    return _connections.size();
}

SessionEntity * ConnectionsManager::child(int row)
{
    return _connections.value(row).get();
}

int ConnectionsManager::indexOf(SessionEntity * session) const
{
    return _connections.indexOf(session->retain());
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
    Q_ASSERT(index < _userQueries.size());
    return _userQueries.at(index);
}

UserQuery * ConnectionsManager::appendNewUserQuery()
{
    auto userQuery = new UserQuery(this);
    _userQueries.push_back(userQuery);
    return userQuery;
}

bool ConnectionsManager::removeUserQueryAt(size_t index)
{
    if (index >= _userQueries.size()) {
        return false;
    }
    UserQuery * userQuery = _userQueries.at(index);
    delete userQuery;
    _userQueries.erase(_userQueries.begin() + index); // who did this with C++?
    return true;
}

void ConnectionsManager::createNewEntity(Entity::Type type)
{
    Q_ASSERT(type > meow::db::Entity::Type::Database);

    Entity * currentEntity = _activeEntity.currentEntity();
    if (!currentEntity) return;

    DataBaseEntity * databaseEntity =
        static_cast<DataBaseEntity *>(
            db::findParentEntityOfType(currentEntity, Entity::Type::Database)
    );
    if (!databaseEntity) return;

    std::shared_ptr<EntityInDatabase> entity
            = EntityFactory::createEntityInDatabase("", type, databaseEntity);

    if (!entity) return;

    entity->setIsNew(true);

    if (entity->type() == meow::db::Entity::Type::Table) {
        // TODO why it is here?
        std::static_pointer_cast<TableEntity>(entity)
                ->setEngine(entity->connection()->defaultTableEngine());
    }

    emit creatingNewEntity(std::static_pointer_cast<Entity>(entity));

}

void ConnectionsManager::refreshActiveSession()
{
    if (_activeSession) {
        setActiveEntity(nullptr);
        _activeSession->refreshAllEntities();
        emit activeSessionRefreshed();
    }
}

bool ConnectionsManager::dropActiveEntity() // TODO: rm?
{
    Entity * activeEntity = _activeEntity.currentEntity();
    if (!activeEntity) return false;

    return dropEntity(activeEntity);
}

bool ConnectionsManager::dropEntity(Entity * entity)
{
    if (_activeSession) {
        if ( (int)entity->type() >= (int)Entity::Type::Table) {
            EntityInDatabase * entityDbLvl
                = static_cast<EntityInDatabase *>(entity);
            return _activeSession->dropEntityInDB(entityDbLvl);
        } else if (entity->type() == Entity::Type::Database) {
            DataBaseEntity * database
                 = static_cast<DataBaseEntity *>(entity);
            return _activeSession->dropDatabase(database);
        } else {
            Q_ASSERT(0);
        }
    }
    return false;
}

QString ConnectionsManager::activeEntityPath() const
{
    QStringList path;

    Entity * entity = _activeEntity.currentEntity();

    while (entity != nullptr) {
        path.prepend(entity->name());
        entity = entity->parent();
    }

    return path.join('/');
}

void ConnectionsManager::setActiveEntity(const db::EntityPtr & activeEntity,
                                         bool select)
{
    bool changed = _activeEntity.setCurrentEntity(activeEntity);
    if (!changed) {
        return;
    }

    if (!activeEntity) {
        Connection * connection = activeConnection();
        if (_activeEntity.databaseChanged() && connection) {
            connection->setDatabase(QString());
        }
        if (_activeEntity.connectionChanged()) {
            setActiveSession(nullptr);
        }
    }

    db::Entity * entity = activeEntity.get();

    if (activeEntity) {
        if (_activeEntity.connectionChanged()) {
            setActiveSession(db::sessionForEntity(entity));
        }
    }

    Connection * connection = activeConnection();

    if (connection && activeEntity) {
        if (_activeEntity.databaseChanged()) {
            QString dbName = databaseName(entity);
            if (dbName.length()) {
                connection->setDatabase(dbName);
            }
        }

        // TODO: use single method for parsing structure
        if (activeEntity->type() == Entity::Type::Table) {
            TableEntity * table = static_cast<TableEntity *>(entity);
            connection->parseTableStructure(table);
        } else if (activeEntity->type() == Entity::Type::View) {
            // TODO: don't parse it here, do when tab is opened?
            if (connection->features()->supportsViewingViews()) {
                ViewEntity * view = static_cast<ViewEntity *>(entity);
                connection->parseViewStructure(view);
            }
        } else if (activeEntity->type() == Entity::Type::Procedure
                   || activeEntity->type() == Entity::Type::Function) {
            if (connection->features()->supportsViewingRoutines()) {
                auto routine = static_cast<RoutineEntity *>(entity);
                connection->parseRoutineStructure(routine);
            }
        } else if (activeEntity->type() == Entity::Type::Trigger) {
            auto trigger = static_cast<TriggerEntity *>(entity);
            connection->parseTriggerStructure(trigger);
        }
    }
    emit activeEntityChanged(activeEntity, select);
}

void ConnectionsManager::onEntityEdited(Entity * entity)
{
    emit entityEdited(entity);
}

void ConnectionsManager::onEntityInserted(const EntityPtr & entity)
{
    emit entityInserted(entity.get());
    setActiveEntity(entity, true);
}

void ConnectionsManager::setActiveSession(SessionEntity * session)
{
    if (_activeSession != session) {
        _activeSession = session;
        meowLogC(Log::Category::Info) << "Entering session: "
                 <<  (session ? session->name() : "");
        emit activeSessionChanged();
    }
}

} // namespace db
} // namespace meow
