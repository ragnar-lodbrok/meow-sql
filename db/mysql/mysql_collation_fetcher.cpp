#include "mysql_collation_fetcher.h"
#include "db/connection.h"

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

QString MySQLCollationFetcher::fetchServerDefaultCollation()
{
    const QString SQL = "SHOW VARIABLES LIKE " +
                         _connection->escapeString("collation_server");
    return _connection->getCell(SQL, 1);
}

QString MySQLCollationFetcher::serverPrefferedCollation()
{
    // should be default from the server, but let's force utf
    // as servers sometimes misconfigured
    if (_connection->serverVersionInt() >= 5*10000 + 5*100 + 3) {
        return "utf8mb4_unicode_ci";
    }
    return "utf8_unicode_ci";
}

} // namespace db
} // namespace meow
