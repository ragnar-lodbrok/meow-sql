#ifndef DB_PG_QUERY_DATA_FETCHER_H
#define DB_PG_QUERY_DATA_FETCHER_H

#include "db/query_data_fetcher.h"

namespace meow {
namespace db {

class PGConnection;

class PGQueryDataFetcher : public QueryDataFetcher
{
public:
    PGQueryDataFetcher(PGConnection * connection);

    virtual void run(QueryCriteria * queryCriteria,
                     QueryData * toData) override;

    // TODO:
    // virtual QStringList selectList(TableEntity * table) override;
};

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_DATA_FETCHER_H
