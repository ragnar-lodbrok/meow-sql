#include "mysql_connection_query_killer.h"
#include "db/connection.h"

namespace meow {
namespace db {

MySQLConnectionQueryKiller::MySQLConnectionQueryKiller(Connection * connection)
    : ConnectionQueryKiller(connection)
{

}

QString MySQLConnectionQueryKiller::killQueryStatement() const
{
    QString killQuery = _connection->serverVersionInt() >= 50000
            ? "KILL QUERY %1"
            : "KILL %1";

    return QString(killQuery).arg(_connection->connectionIdOnServer());
}

} // namespace db
} // namespace meow
