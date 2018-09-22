#ifndef DB_QUERY_DATA_H
#define DB_QUERY_DATA_H

#include "db/connection.h"
#include "db/data_type/data_type_category.h"

namespace meow {
namespace db {

// Provides a nicer API for Query resuls with ability to edit query data results
class QueryData
{
public:
    QueryData();

    db::Query * query() const { return _queryPtr.get(); }
    void setQueryPtr(db::QueryPtr queryPtr) { _queryPtr = queryPtr;
                                              _curRowNumber = -1; }
    void clearData() { setQueryPtr(nullptr); }

    int rowCount() const;
    int columnCount() const;
    QString rawDataAt(int row, int column) const;
    bool isNullAt(int row, int column) const;
    QString columnName(int index) const;
    db::DataTypeCategoryIndex columnDataTypeCategory(int index) const;

    void prepareEditing();
    bool setData(int row, int col, const QVariant &value);
    bool isModified() const;

    int applyModifications();
    int discardModifications();

    QString whereForCurRow(bool notModified = false) const;
    void ensureFullRow(bool refresh = false);

    void setCurrentRowNumber(int row);
    int currentRowNumber() const { return _curRowNumber; }

private:

    bool hasFullData() const;

    db::QueryPtr _queryPtr;
    int _curRowNumber;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_H
