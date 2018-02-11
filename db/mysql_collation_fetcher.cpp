#include "mysql_collation_fetcher.h"
#include "connection.h"

namespace meow {
namespace db {

MySQLCollationFetcher::MySQLCollationFetcher(Connection * connection)
    : CollationFetcher(connection)
{

}

QStringList MySQLCollationFetcher::run()
{

    if (_connection->serverVersionInt() >= 40100) {
        return _connection->getColumn("SHOW COLLATION", 0);
    }
    return {};
}

} // namespace db
} // namespace meow
