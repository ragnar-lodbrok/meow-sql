#ifndef DB_CONNECTIONS_MANAGER_H
#define DB_CONNECTIONS_MANAGER_H

#include <QVector>
#include "connection_parameters.h"

namespace meow {
namespace db {

// Intent: holds active db connections
class ConnectionsManager
{
public:
    ConnectionsManager();
    ConnectionPtr openDBConnection(db::ConnectionParameters & params);
private:
    QVector<ConnectionPtr> _connections;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTIONS_MANAGER_H
