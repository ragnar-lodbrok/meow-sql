#include "connections_manager.h"
#include "connection.h"

namespace meow {
namespace db {

ConnectionsManager::ConnectionsManager()
    :_connections()
{

}

ConnectionPtr ConnectionsManager::openDBConnection(db::ConnectionParameters & params)
{
    ConnectionPtr connection = params.createConnection();

    connection->setActive(true);

    _connections.push_back(connection);

    return connection;
}

} // namespace db
} // namespace meow
