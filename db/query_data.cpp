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
}


} // namespace db
} // namespace meow
