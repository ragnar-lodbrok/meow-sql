#ifndef DB_QUERY_DATA_FETCHER_H
#define DB_QUERY_DATA_FETCHER_H

#include <QStringList>

namespace meow {
namespace db {

class Connection;
class QueryCriteria;
class QueryData;
class TableEntity;
class TableColumn;

// TODO: rename to TableDataFetcher or split into 2 classes ?
class QueryDataFetcher
{
public:
    QueryDataFetcher(Connection * connection);
    virtual ~QueryDataFetcher() {}

    virtual void run(QueryCriteria * queryCriteria,
                     QueryData * toData) = 0;

    virtual QStringList selectList(TableEntity * table) {
        Q_UNUSED(table);
        QStringList select;
        select << "*";
        return select;
    }

protected:

    QList<meow::db::TableColumn *> partLoadColumns(TableEntity * table);

    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_FETCHER_H
