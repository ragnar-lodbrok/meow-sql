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

} // namespace db
} // namespace meow
