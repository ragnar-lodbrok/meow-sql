#ifndef DB_QUERY_DATA_H
#define DB_QUERY_DATA_H

#include <QObject>
#include <QList>
#include <QPair>
#include "db/connection.h"
#include "db/data_type/data_type_category.h"
#include "query.h"
#include "editable_grid_data.h"

namespace meow {
namespace db {

// id => value
using IdValueList = QList<QPair<QString, QString>>;

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

    size_t resultCount() const {
        if (_queryPtr) {
            return _queryPtr->resultCount();
        }
        return 0;
    }
    void setResultIndex(size_t resultIndex) { _resultIndex = resultIndex; }

    inline QueryResultPt currentResult() const {
        return _queryPtr->resultAt(_resultIndex);
    }

    int rowCount() const;
    int columnCount() const;
    QString displayDataAt(int row, int column) const;
    QVariant editDataAt(int row, int column) const;
    bool isNullAt(int row, int column) const;
    QString columnName(int index) const;
    db::DataTypeCategoryIndex columnDataTypeCategory(int index) const;
    db::DataTypePtr dataTypeForColumn(int column) const;
    bool columnHasForeignKey(int column) const;
    bool columnIsPrimaryKeyPart(std::size_t index) const;
    bool columnIsUniqueKeyPart(std::size_t index) const;
    bool columnIsIndexKeyPart(std::size_t index) const;

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
    int duplicateCurrentRowWithoutKeys();
    int duplicateCurrentRowWithKeys();
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

    QVariant editDataForForeignKey(ForeignKey * fKey,
                                   const QString & columnName) const;

    db::QueryPtr _queryPtr;
    int _curRowNumber;
    size_t _resultIndex;
};

using QueryDataPtr = std::shared_ptr<QueryData>;

} // namespace db
} // namespace meow

Q_DECLARE_METATYPE(meow::db::IdValueList);

#endif // DB_QUERY_DATA_H
