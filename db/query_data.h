#ifndef DB_QUERY_DATA_H
#define DB_QUERY_DATA_H

#include "db/connection.h"
#include "db/data_type/data_type_category.h"

namespace meow {
namespace db {

// Provides a nicer API for Query resuls
class QueryData
{
public:
    QueryData();

    db::Query * query() const { return _queryPtr.get(); }
    void setQueryPtr(db::QueryPtr queryPtr) { _queryPtr = queryPtr; }

    int rowCount() const;
    int columnCount() const;
    QString rawDataAt(int row, int column) const;
    bool isNullAt(int row, int column) const;
    QString columnName(int index) const;
    db::DataTypeCategoryIndex columnDataTypeCategory(int index) const;

private:
    db::QueryPtr _queryPtr;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_H
