#include "pg_query_data_fetcher.h"
#include "pg_connection.h"
#include "db/query_criteria.h"
#include "db/query_data.h"
#include "db/query.h"

namespace meow {
namespace db {

PGQueryDataFetcher::PGQueryDataFetcher(PGConnection * connection)
    : QueryDataFetcher(connection)
{

}

void PGQueryDataFetcher::run(
        QueryCriteria * queryCriteria,
        QueryData * toData) // override
{

    // TODO: the code is the same as in MySQLQueryDataFetcher

    QString selectList = queryCriteria->select.join(", ");
    if (selectList.isEmpty()) {
        selectList = "*";
    }
    QString select = selectList
            + " FROM " + queryCriteria->quotedDbAndTableName;

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
