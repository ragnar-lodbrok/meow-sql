#ifndef DB_CONNECTIONS_MANAGER_H
#define DB_CONNECTIONS_MANAGER_H

#include <QObject>
#include <QList>
#include <vector>
#include "connection_parameters.h"
#include "entity/session_entity.h"
#include "db/entity/entity_holder.h"

namespace meow {
namespace db {

class UserQuery;

// Intent: holds active db connections
class ConnectionsManager : public QObject, public Entity // root db entity
{
    Q_OBJECT

public:
    ConnectionsManager();
    ~ConnectionsManager();

    ConnectionPtr openDBConnection(db::ConnectionParameters & params);

    // Entity interface
    virtual int childCount() override;
    virtual SessionEntity * child(int row) override;
    int indexOf(SessionEntity * session) const;
    // Entity interface (end)

    Entity * activeEntity() const { return _activeEntity.currentEntity(); }
    void setActiveEntity(Entity * activeEntity, bool select = false);

    Connection * activeConnection() const;
    SessionEntity * activeSession() const { return _activeSession; }
    const QList <SessionEntity *> & sessions() const { return _connections; }

    UserQuery * userQueryAt(size_t index);

    bool isNoOpenedConnections() const { return _connections.isEmpty(); }

    Q_SIGNAL void connectionOpened(SessionEntity * newSession);
    Q_SIGNAL void activeEntityChanged(Entity * newEntity, bool select = false);
    Q_SIGNAL void creatingNewEntity(Entity * entity);
    Q_SIGNAL void entityEdited(Entity * entity);
    Q_SIGNAL void entityInserted(Entity * entity);
    Q_SIGNAL void activeSessionRefreshed();
    Q_SIGNAL void activeDatabaseChanged(const QString & database);

    void createEntity(Entity::Type type);

    void refreshActiveSession();

    bool dropActiveEntity();

    QString activeEntityPath() const;

private:

    Q_SLOT void onEntityEdited(Entity * entity);
    Q_SLOT void onEntityInserted(Entity * entity);

    QList <SessionEntity *> _connections;
    EntityHolder _activeEntity;
    SessionEntity * _activeSession;
    std::vector <UserQuery *> _userQueries;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTIONS_MANAGER_H
