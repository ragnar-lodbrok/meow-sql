#include "query_data.h"
#include "editable_grid_data.h"
#include "query.h"
#include "helpers/formatting.h"
#include "query_data_editor.h"
#include "entity/entity.h"
#include <QDebug>

namespace meow {
namespace db {

QueryData::QueryData() : _curRowNumber(-1)
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

// TODO: why it's called raw when it returns formatted data ?
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
    setCurrentRowNumber(row);
    db::Query * query = _queryPtr.get();
    if (query) {
        query->prepareEditing();
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

int QueryData::applyModifications()
{
    if (!isModified()) return -1;

    std::shared_ptr<QueryDataEditor> editor = query()->connection()
                                                     ->queryDataEditor();

    editor->applyModificationsInDB(this);

    ensureFullRow(true); // load from db new values

    return _queryPtr->editableData()->applyModifications();
}

int QueryData::discardModifications()
{
    if (!isModified()) return -1;

    return _queryPtr->editableData()->discardModifications();
}

QString QueryData::whereForCurRow(bool notModified) const
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

        const QString & value =  notModified ?
                    editableData->notModifiedDataAt(row, i)
                  : editableData->dataAt(row, i);

        QString whereVal;

        if (value.isNull()) {
            whereVal = " IS NULL";
        } else {
            switch (query()->column(i).dataTypeCategoryIndex) {
            case DataTypeCategoryIndex::Integer:
            case DataTypeCategoryIndex::Float:
                // TODO if bit
                whereVal = value.isEmpty() ? "0" : value;
                break;
            // TODO: other types
            default:
                whereVal = query()->connection()->escapeString(value);
                break;
            }

            whereVal = '=' + whereVal;
        }

        whereList << whereName + whereVal;
    }

    return whereList.join(" AND");
}

void QueryData::ensureFullRow(bool refresh)
{
    if (!refresh && hasFullData()) return;

    prepareEditing();

    EditableGridDataRow * row = _queryPtr->editableData()->editableRow();
    Q_ASSERT(row != nullptr); // TODO create when not

    QStringList columnNames = query()->connection()->quoteIdentifiers(
        query()->columnOrgNames()
    );

    QString entityName = db::quotedFullName(query()->entity());
    QString selectSQL = QString("SELECT %1 FROM %2 WHERE %3 %4")
            .arg(columnNames.join(", "))
            .arg(entityName)
            .arg(whereForCurRow())
            .arg(query()->connection()->limitOnePostfix());

    // TODO: ignore if fail ?
    QStringList newRowData = query()->connection()->getRow(selectSQL);

    row->data = newRowData;
}

void QueryData::setCurrentRowNumber(int row)
{
    _curRowNumber = row;
}

bool QueryData::hasFullData() const
{
    if (isModified()) return true;

    // TODO
    return true;
}

} // namespace db
} // namespace meow
