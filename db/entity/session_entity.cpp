#include "session_entity.h"
#include "db/connections_manager.h"
#include "database_entity.h"
#include "table_entity.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace db {

SessionEntity::SessionEntity(ConnectionPtr connection, ConnectionsManager * parent)
    :QObject(nullptr),
     Entity(parent),
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
    static const QIcon icon = QIcon(":/icons/server-mysql.png");

    return icon;
}

int SessionEntity::indexOf(DataBaseEntity * session) const
{
    return _databases.indexOf(session);
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

void SessionEntity::editTableInDB(TableEntity * table, TableEntity * newData)
{
    bool changed = connection()->editTableInDB(table, newData);
    if (changed) {
        table->copyData(newData);
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

        QStringList databaseNames = connection()->allDatabases();

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

} // namespace db
} // namespace meow
