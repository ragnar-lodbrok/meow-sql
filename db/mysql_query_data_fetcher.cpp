#include "mysql_query_data_fetcher.h"
#include "query_criteria.h"
#include "query_data.h"
#include "query.h"
#include "mysql_connection.h"
#include <QDebug>

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

    QString select = "* FROM " + queryCriteria->quotedDbAndTableName;

    select = _connection->applyQueryLimit("SELECT", select,
                                          queryCriteria->limit,
                                          queryCriteria->offset);

    Query * query = toData->query();
    if (query == nullptr) {
        QueryPtr newQuery = _connection->createQuery();
        toData->setQueryPtr(newQuery);
        query = toData->query();
    }

    query->setSQL(select);

    query->execute(queryCriteria->offset > 0);
}

} // namespace db
} // namespace meow
