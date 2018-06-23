#ifndef MYSQL_QUERY_DATA_FETCHER_H
#define MYSQL_QUERY_DATA_FETCHER_H

#include "query_data_fetcher.h"

namespace meow {
namespace db {

class MySQLConnection;

class MySQLQueryDataFetcher : public QueryDataFetcher
{
public:

    MySQLQueryDataFetcher(MySQLConnection * connection);

    virtual void run(QueryCriteria * queryCriteria,
                     QueryData * toData) override;

    virtual QStringList selectList(TableEntity * table) override;
};

} // namespace db
} // namespace meow

#endif // MYSQL_QUERY_DATA_FETCHER_H
