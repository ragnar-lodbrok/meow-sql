#include "query_data_sort_filter_proxy_model.h"
#include "db/query_data.h"

namespace meow {
namespace models {
namespace db {

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

} // namespace db
} // namespace models
} // namespace meow
