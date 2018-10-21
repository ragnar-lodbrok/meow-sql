#include "query_data.h"
#include "editable_grid_data.h"
#include "query.h"
#include "helpers/formatting.h"
#include "query_data_editor.h"
#include "entity/entity.h"
#include <QDebug>

namespace meow {
namespace db {

QueryData::QueryData() : QObject(), _curRowNumber(-1)
{

}

int QueryData::rowCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return static_cast<int>(query->recordCount());
    }
    return 0;
}

int QueryData::columnCount() const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return static_cast<int>(query->columnCount());
    }
    return 0;
}

QString QueryData::columnName(int index) const
{
    db::Query * query = _queryPtr.get();
    if (query) {
        return query->columnName(static_cast<std::size_t>(index));
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
        query->seekRecNo(static_cast<std::size_t>(row));
        return query->isNull(static_cast<std::size_t>(column));
    }
    return false;
}

bool QueryData::setData(int row, int col, const QVariant &value)
{
    setCurrentRowNumber(row);
    db::Query * query = _queryPtr.get();
    if (query) {
        prepareEditing();
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
        emit editingPrepared();
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

QString QueryData::whereForCurRow(bool beforeModifications) const
{
    QStringList whereList;

    QStringList keyColumns = query()->keyColumns();

    bool useEditableData = false;
    std::size_t row = static_cast<std::size_t>(_curRowNumber);

    EditableGridData * editableData = query()->editableData();
    if (editableData && editableData->editableRow()) {
        row = editableData->editableRow()->rowNumber;
        useEditableData = true;
    }

    std::size_t columnCount = query()->columnCount();

    for (const QString & keyColumnName : keyColumns) {
        std::size_t i = static_cast<std::size_t>(-1);
        for (i = 0; i < columnCount; ++i) {
            if (keyColumnName == query()->columnName(i)) {
                break;
            }
        }
        if (i == static_cast<std::size_t>(-1)) {
            throw db::Exception(
                QString("Cannot compose WHERE clause - column missing: %1")
                    .arg(keyColumnName));
        }
        QString whereName = query()->connection()->quoteIdentifier(
            query()->columnName(i)
        );

        QString value;

        if (useEditableData) {
            value = beforeModifications ?
                        editableData->notModifiedDataAt(row, i)
                      : editableData->dataAt(row, i);
        } else {
            db::Query * query = _queryPtr.get();
            Q_ASSERT(query != nullptr);
            query->seekRecNo(row);
            value = query->curRowColumn(i);
        }

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
