#include "pg_connection_query_killer.h"
#include "db/connection.h"

namespace meow {
namespace db {

PGConnectionQueryKiller::PGConnectionQueryKiller(Connection * connection)
    : ConnectionQueryKiller(connection)
{

}

QString PGConnectionQueryKiller::killQueryStatement() const
{
    // TODO: not tested
    return QString("SELECT pg_cancel_backend(%1)")
            .arg(_connection->connectionIdOnServer());
}

} // namespace db
} // namespace meow
