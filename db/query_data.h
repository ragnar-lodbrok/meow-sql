#ifndef DB_QUERY_DATA_H
#define DB_QUERY_DATA_H

#include "db/connection.h"

namespace meow {
namespace db {

// Provides a nicer API for Query resuls
class QueryData
{
public:
    QueryData();

    db::Query * query() const { return _queryPtr.get(); }
    void setQueryPtr(db::QueryPtr queryPtr) { _queryPtr = queryPtr; }

private:
    db::QueryPtr _queryPtr;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_H
