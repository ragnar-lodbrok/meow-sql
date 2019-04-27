#include "session_entity.h"
#include "db/connections_manager.h"
#include "database_entity.h"
#include "table_entity.h"
#include "helpers/logger.h"
#include "app/app.h"
#include "db/connection.h"

namespace meow {
namespace db {

SessionEntity::SessionEntity(std::shared_ptr<Connection> connection,
                             ConnectionsManager * parent)
    : Entity(parent),
     _connection(connection),
     _databases(),
     _databasesWereInit(false)
{

}

SessionEntity::~SessionEntity()
{
    meowLogDebug() << "Closing session " << name();
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
    return static_cast<ConnectionsManager *>(_parent);
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
    auto icons = meow::app()->settings()->icons();
    return icons->connection(_connection->connectionParams()->serverType());

}

int SessionEntity::indexOf(DataBaseEntity * session) const
{
    return _databases.indexOf(session);
}

bool SessionEntity::isActive() const
{
    return connectionsManager()->activeSession() == this;
}

DataBaseEntity * SessionEntity::activeDatabase() const
{
    foreach (DataBaseEntity * entity, _databases) {
        if (entity->name() == connection()->database()) {
            return entity;
        }
    }
    return nullptr;
}

void SessionEntity::clearAllDatabaseEntities()
{
    qDeleteAll(_databases);
    _databases.clear();
    _databasesWereInit = false;
}

void SessionEntity::refreshAllEntities()
{
    clearAllDatabaseEntities();
    initDatabasesListIfNeed();
}

void SessionEntity::editTableInDB(TableEntity * table,
                                  TableEntity * newData)
{
    bool changed = connection()->editTableInDB(table, newData);
    if (changed) {
        table->copyDataFrom(newData);
        emit entityEdited(table);
    }
}

void SessionEntity::insertTableToDB(TableEntity * table)
{
    if (connection()->insertTableToDB(table)) {
        table->setIsNew(false);
        addEntity(table);
        emit entityInserted(table);
    }
}

bool SessionEntity::dropEntityInDB(EntityInDatabase * entity)
{
    return connection()->dropEntityInDB(entity);
}

bool SessionEntity::dropDatabase(DataBaseEntity * database)
{
    return connection()->dropDatabase(database);
}

void SessionEntity::createDatabase(const QString & name,
                                   const QString & collation)
{
    _connection->createDatabase(name, collation);

    if (_connection->connectionParams()->isAllDatabases()) {

        QStringList allDatabases = _connection->allDatabases(true);
        if (allDatabases.contains(name) == false) return;

        initDatabasesListIfNeed();
        DataBaseEntity * dbEntity = new DataBaseEntity(
                    name,
                    const_cast<SessionEntity *>(this));
        _databases.push_back(dbEntity);

        emit entityInserted(dbEntity);
    }
}

db::ulonglong SessionEntity::dataSize() const // override
{
    db::ulonglong sum = 0;

    foreach (const DataBaseEntity * entity, _databases) {
        sum += entity->dataSize();
    }

    return sum;
}

void SessionEntity::initDatabasesListIfNeed()
{
    if (_databasesWereInit == false) {

        QStringList databaseNames = connection()->allDatabases(true);

        foreach (const QString &dbName, databaseNames) {
            DataBaseEntity * dbEntity = new DataBaseEntity(
                        dbName,
                        const_cast<SessionEntity *>(this));
            _databases.push_back(dbEntity);
        }

        _databasesWereInit = true;
    }
}

void SessionEntity::addEntity(Entity * entity)
{
    if ( (int)entity->type() >= (int)Entity::Type::Table ) {
        EntityInDatabase * entityInDb = static_cast<EntityInDatabase *>(entity);
        entityInDb->dataBaseEntity()->appendEntity(entityInDb);
    }
}

bool SessionEntity::removeEntity(Entity * entity)
{
    // Listening: The Agonist - Business Suits And Combat Boots

    bool success = false;

    emit beforeEntityRemoved(entity);
    if ( (int)entity->type() >= (int)Entity::Type::Table ) {
        EntityInDatabase * entityInDb = static_cast<EntityInDatabase *>(entity);
        success = entityInDb->dataBaseEntity()->removeEntity(entityInDb);
    } else if (entity->type() == Entity::Type::Database) {
        DataBaseEntity * database = static_cast<DataBaseEntity *>(entity);
        _databases.removeOne(database);
        success = true;
    } else {
        Q_ASSERT(0);
    }

    emit enitityRemoved(entity);
    entity->deleteLater();

    return success;
}

} // namespace db
} // namespace meow
