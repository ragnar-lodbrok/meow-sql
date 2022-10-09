#include "query_data_sort_filter_proxy_model.h"
#include "db/query_data.h"

namespace meow {
namespace ui {
namespace models {

QueryDataSortFilterProxyModel::QueryDataSortFilterProxyModel(
        meow::db::QueryData * queryData,
        QObject *parent)

    : QSortFilterProxyModel(parent)
    , _queryData(queryData)
{

}

QueryDataSortFilterProxyModel::~QueryDataSortFilterProxyModel()
{

}

bool QueryDataSortFilterProxyModel::filterAcceptsRow(
        int sourceRow,
        const QModelIndex &sourceParent) const
{
     if (_queryData->isRowInsertedButNotSaved(sourceRow)) {
         return true; // always show new inserted rows for editing
     }
     return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

bool QueryDataSortFilterProxyModel::lessThan(
        const QModelIndex &left,
        const QModelIndex &right) const
{
    // Use natural sort for numeric types and default for other

    db::DataTypeCategoryIndex columnType
            = _queryData->columnDataTypeCategory(left.column());

    switch (columnType) {

    case db::DataTypeCategoryIndex::Integer: {
        // Use biggest int type so even huge values don't overflow
        // TODO: still overflows if uint64_t and value > max<int64_t>()
        qlonglong leftData = sourceModel()->data(left).toLongLong();
        qlonglong rightData = sourceModel()->data(right).toLongLong();;

        return leftData < rightData;
    }

    case db::DataTypeCategoryIndex::Float: {
        // Use double not float so even huge values don't overlow
        double leftData = sourceModel()->data(left).toDouble();
        double rightData = sourceModel()->data(right).toDouble();;

        return leftData < rightData;
    }

    default:
        return QSortFilterProxyModel::lessThan(left, right);
    }
}

} // namespace models
} // namespace ui
} // namespace meow
