#ifndef DB_CONNECTIONS_MANAGER_H
#define DB_CONNECTIONS_MANAGER_H

#include <QObject>
#include <QList>
#include "connection_parameters.h"
#include "entity/session_entity.h"

namespace meow {
namespace db {

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

    Entity * activeEntity() const { return _activeEntity; }
    void setActiveEntity(Entity * activeEntity);

    Q_SIGNAL void connectionOpened(SessionEntity * newSession);
    Q_SIGNAL void activeEntityChanged(Entity * newEntity);

private:
    QList <SessionEntity *> _connections;
    Entity * _activeEntity;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTIONS_MANAGER_H
