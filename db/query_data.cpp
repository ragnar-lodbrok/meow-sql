#include "query_data.h"
#include "editable_grid_data.h"
#include "query.h"
#include "helpers/formatting.h"
#include "query_data_editor.h"
#include <QDebug>

namespace meow {
namespace db {

QueryData::QueryData()
{

}

int QueryData::rowCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->recordCount();
    }
    return 0;
}

int QueryData::columnCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->columnCount();
    }
    return 0;
}

QString QueryData::columnName(int index) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->columnName(index);
    }
    return QString();
}

DataTypeCategoryIndex QueryData::columnDataTypeCategory(int index) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->column(index).dataTypeCategoryIndex;
    }
    return DataTypeCategoryIndex::Other;
}

QString QueryData::rawDataAt(int row, int column) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        query->seekRecNo(row);
        if (query->isNull(column)) {
            return "(NULL)"; // TODO: const
        } else {

            QString data = query->curRowColumn(column, true);

            // TODO: more formatting, see AnyGridGetText

            auto categoryIndex = columnDataTypeCategory(column);
            if (categoryIndex == DataTypeCategoryIndex::Spatial
                || categoryIndex == DataTypeCategoryIndex::Binary) {
                return helpers::formatAsHex(data);
            }

            return data;
        }
    }
    return QString();
}

bool QueryData::isNullAt(int row, int column) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        query->seekRecNo(row);
        return query->isNull(column);
    }
    return false;
}

bool QueryData::setData(int row, int col, const QVariant &value)
{
    db::Query * query = _queryPtr.get();
    if (query) {
        Q_ASSERT(query->editableData());
        query->editableData()->setData(row, col, value);
    }

    return false;
}

void QueryData::prepareEditing()
{
    db::Query * query = _queryPtr.get();
    if (query) {
        query->prepareEditing();
    }
}

bool QueryData::isModified() const
{
    return _queryPtr
            && _queryPtr->editableData()
            && _queryPtr->editableData()->isModified();
}

void QueryData::applyModifications()
{
    if (!isModified()) return;

    std::shared_ptr<QueryDataEditor> editor = query()->connection()
                                                     ->queryDataEditor();

    editor->applyModificationsInDB(this);

    _queryPtr->editableData()->applyModifications();
}

QString QueryData::whereForCurRow() const
{
    QStringList whereList;

    QStringList keyColumns = query()->keyColumns();

    EditableGridData * editableData = query()->editableData();
    std::size_t row = editableData->editableRow()->rowNumber;
    // TODO use cur row when no modifications

    int columnCount = query()->columnCount();

    for (const QString & keyColumnName : keyColumns) {
        int i = -1;
        for (i = 0; i < columnCount; ++i) {
            if (keyColumnName == query()->columnName(i)) {
                break;
            }
        }
        if (i == -1) {
            throw db::Exception(
                QString("Cannot compose WHERE clause - column missing: %1")
                    .arg(keyColumnName));
        }
        QString whereName = query()->connection()->quoteIdentifier(
            query()->columnName(i)
        );

        const QString & oldValue = editableData->notModifiedDataAt(row, i);

        QString whereVal;

        if (oldValue.isNull()) {
            whereVal = " IS NULL";
        } else {
            switch (query()->column(i).dataTypeCategoryIndex) {
            case DataTypeCategoryIndex::Integer:
            case DataTypeCategoryIndex::Float:
                // TODO if bit
                whereVal = oldValue.isEmpty() ? "0" : oldValue;
                break;
            // TODO: other types
            default:
                whereVal = query()->connection()->escapeString(oldValue);
                break;
            }

            whereVal = '=' + whereVal;
        }

        whereList << whereName + whereVal;
    }

    return whereList.join(" AND");
}

} // namespace db
} // namespace meow
