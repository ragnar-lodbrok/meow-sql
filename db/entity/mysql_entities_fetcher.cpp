#include "mysql_entities_fetcher.h"
#include "db/mysql_connection.h"
#include <QDebug>

namespace meow {
namespace db {

MySQLEntitiesFetcher::MySQLEntitiesFetcher(MySQLConnection * connection)
    :DataBaseEntitiesFetcher(connection)
{

}

void MySQLEntitiesFetcher::run(const QString & dbName, EntityListForDataBase * toList) // override
{
    MySQLConnection * connection = static_cast<MySQLConnection *>(_connection);

    bool fullTableStatus = connection->connectionParams()->fullTableStatus();

    if (fullTableStatus || (QString::compare(dbName, "INFORMATION_SCHEMA", Qt::CaseInsensitive) == 0)) {
        //QueryPtr query = connection->getResults("SHOW TABLE STATUS FROM " + connection->quoteIdentifier(dbName));
    } else {
        qDebug() << "Not implemented";
    }
}


} // namespace db
} // namespace meow

