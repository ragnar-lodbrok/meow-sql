#include "mysql_query_data_fetcher.h"
#include "query_criteria.h"
#include "query_data.h"
#include "query.h"
#include "mysql_connection.h"

namespace meow {
namespace db {

MySQLQueryDataFetcher::MySQLQueryDataFetcher(MySQLConnection * connection)
    :QueryDataFetcher(connection)
{

}

void MySQLQueryDataFetcher::run(
        QueryCriteria * queryCriteria,
        QueryData * toData) // override
{
    // just test stuff
    QString select = "SELECT * FROM " + queryCriteria->quotedDbAndTableName;

    Query * query = toData->query();
    if (query == nullptr) {
        QueryPtr newQuery = _connection->createQuery();
        toData->setQueryPtr(newQuery);
        query = toData->query();
    }

    query->setSQL(select);

    query->execute();
}

} // namespace db
} // namespace meow
