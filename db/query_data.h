#ifndef DB_QUERY_DATA_H
#define DB_QUERY_DATA_H

#include <QObject>
#include "db/connection.h"
#include "db/data_type/data_type_category.h"
#include "query.h"
#include "editable_grid_data.h"

namespace meow {
namespace db {

// Provides a nicer API for Query resuls with ability to edit query data results
class QueryData : public QObject
{
    Q_OBJECT

public:
    QueryData();

    db::Query * query() const { return _queryPtr.get(); }
    void setQueryPtr(db::QueryPtr queryPtr) { _queryPtr = queryPtr;
                                              _curRowNumber = -1; }
    void clearData() { setQueryPtr(nullptr); }

    int rowCount() const;
    int columnCount() const;
    QString displayDataAt(int row, int column) const;
    QString editDataAt(int row, int column) const;
    bool isNullAt(int row, int column) const;
    QString columnName(int index) const;
    db::DataTypeCategoryIndex columnDataTypeCategory(int index) const;
    db::DataTypePtr dataTypeForColumn(int column) const;

    void prepareEditing();
    bool setData(int row, int col, const QVariant &value);
    bool isModified() const;
    bool isInserted() const;
    int modifiedRowNumber() const;

    int applyModifications();
    int discardModifications();

    bool deleteRowInDB(int row);
    void deleteRow(int row);
    int insertEmptyRow();
    bool isRowInsertedButNotSaved(int rowNumber) {
        db::Query * query = _queryPtr.get();
        if (!query || !query->isEditing()) {
            return false;
        }
        return query->editableData()->isRowInserted(rowNumber);
    }

    QString whereForCurRow(bool beforeModifications = false) const;
    void ensureFullRow(bool refresh = false);

    void setCurrentRowNumber(int row);
    int currentRowNumber() const { return _curRowNumber; }

    Q_SIGNAL void editingPrepared();

private:

    bool hasFullData() const;

    db::QueryPtr _queryPtr;
    int _curRowNumber;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_DATA_H
