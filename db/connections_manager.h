#ifndef DB_CONNECTIONS_MANAGER_H
#define DB_CONNECTIONS_MANAGER_H

#include <QList>
#include "connection_parameters.h"
#include "entity/session_entity.h"
#include "db/entity/entity_holder.h"
#include "user_queries_manager.h"

#ifdef WITH_MYSQL
#include "db/mysql/mysql_library_initializer.h"
#endif

namespace meow {
namespace db {

// Intent: holds active db connections
class ConnectionsManager : public Entity // root db entity
{
    Q_OBJECT

private:
    ConnectionsManager();
public:
    static std::shared_ptr<ConnectionsManager> create(); // TODO: Singletone
    void init();
    ~ConnectionsManager() override;

    ConnectionPtr openDBConnection(db::ConnectionParameters & params);
    bool closeActiveConnection();

    // Entity interface
    virtual int childCount() override;
    virtual SessionEntity * child(int row) override;
    int indexOf(SessionEntity * session) const;
    // Entity interface (end)

    Entity * activeEntity() const { return _activeEntity.currentEntity(); }
    void setActiveEntity(const EntityPtr & activeEntity, bool select = false);

    Connection * activeConnection() const;
    SessionEntity * activeSession() const { return _activeSession; }
    const QList <SessionEntityPtr> & sessions() const { return _connections; }

    bool isNoOpenedConnections() const { return _connections.isEmpty(); }

    Q_SIGNAL void connectionOpened(SessionEntity * newSession);
    Q_SIGNAL void beforeConnectionClosed(SessionEntity * session);
    Q_SIGNAL void activeEntityChanged(const EntityPtr &newEntity,
                                      bool select = false);
    Q_SIGNAL void creatingNewEntity(const EntityPtr & entity);
    Q_SIGNAL void entityEdited(Entity * entity);
    Q_SIGNAL void entityInserted(Entity * entity);
    Q_SIGNAL void activeSessionChanged();
    Q_SIGNAL void activeSessionRefreshed();
    Q_SIGNAL void activeDatabaseChanged(const QString & database);

    void createNewEntity(Entity::Type type);

    void refreshActiveSession();

    bool dropActiveEntity();
    bool dropEntity(Entity * entity);

    bool emptyEntity(Entity * entity);

    QString activeEntityPath() const;

    const UserQueriesManager * userQueriesManager() const {
        return &_userQueriesManager;
    }
    UserQueriesManager * userQueriesManager() {
        return &_userQueriesManager;
    }

private:

    Q_SLOT void onEntityEdited(Entity * entity);
    Q_SLOT void onEntityInserted(const EntityPtr & entity);

    void setActiveSession(SessionEntity * session);

    QList <SessionEntityPtr> _connections;
    EntityHolder _activeEntity;
    SessionEntity * _activeSession;
    UserQueriesManager _userQueriesManager;

#ifdef WITH_MYSQL
    MySQLLibraryInitializer _mySQLLibInit; // TODO: move to ConnectionParameters ?
#endif
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTIONS_MANAGER_H
