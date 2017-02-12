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

    // Entity
    virtual int childCount() override;
    virtual SessionEntity * child(int row) override;
    int indexOf(SessionEntity * session) const;
    // Entity (end)

    Q_SIGNAL void connectionOpened(SessionEntity * newSession);

private:
    QList <SessionEntity *> _connections;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTIONS_MANAGER_H
