#include "entities_tree_sort_filter_proxy_model.h"
#include "entities_tree_model.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

EntitiesTreeSortFilterProxyModel::EntitiesTreeSortFilterProxyModel(
        QObject * parent)
    : QSortFilterProxyModel(parent)
{
    _databaseFilter.setPatternOptions(
                QRegularExpression::CaseInsensitiveOption);

    _tableFilter.setPatternOptions(_databaseFilter.patternOptions());
}

void EntitiesTreeSortFilterProxyModel::setDatabaseRegexpFilter(
        const QString & filter)
{
    _databaseFilter.setPattern(filter);
    _databaseFilter.optimize();
    invalidate(); // hack to avoid crash that I can't fix
    invalidateFilter();
}

void EntitiesTreeSortFilterProxyModel::setTableRegexpFilter(
        const QString & filter)
{
    _tableFilter.setPattern(filter);
    _tableFilter.optimize();
    //invalidate(); // resets selection, don't use
    invalidateFilter();
}

bool EntitiesTreeSortFilterProxyModel::filterAcceptsRow(
        int sourceRow,
        const QModelIndex &sourceParent) const
{
    if (_databaseFilter.pattern().isEmpty()
            && _tableFilter.pattern().isEmpty()) {
        return true;
    }

    auto model = static_cast<models::EntitiesTreeModel *>(sourceModel());

    return model->filterAcceptsRow(sourceRow, sourceParent,
                                   _databaseFilter, _tableFilter);
}

} // namespace models
} // namespace ui
} // namespace meow
