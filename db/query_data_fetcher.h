#ifndef DB_QUERY_DATA_FETCHER_H
#define DB_QUERY_DATA_FETCHER_H

namespace meow {
namespace db {

class Connection;
class QueryCriteria;
class QueryData;

class QueryDataFetcher
{
public:
    QueryDataFetcher(Connection * connection);
    virtual ~QueryDataFetcher() {}

    virtual void run(QueryCriteria * queryCriteria,
                     QueryData * toData) = 0;
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_FETCHER_H
