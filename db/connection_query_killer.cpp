#include "connection_query_killer.h"
#include "connection.h"
#include <QDebug>

namespace meow {
namespace db {

ConnectionQueryKiller::ConnectionQueryKiller(Connection * connection)
    : _connection(connection)
{
    Q_ASSERT(_connection != nullptr);
}

ConnectionQueryKiller::~ConnectionQueryKiller()
{

}

void ConnectionQueryKiller::run()
{
    if (!_connection->features()->supportsCancellingQuery()) {
        Q_ASSERT(false);
        return;
    }

    ConnectionParameters * params = _connection->connectionParams();

    ConnectionPtr connection = params->createConnection();

    // TODO: H: Killer.LogPrefix := _('Helper connection');

    connection->query(killQueryStatement());
}

QString ConnectionQueryKiller::killQueryStatement() const
{
    // default behaviour
    return QString("KILL %1").arg(_connection->connectionIdOnServer());
}

} // namespace db
} // namespace meow
