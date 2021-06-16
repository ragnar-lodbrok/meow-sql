#ifndef UI_CENTRALLEFTWIDGET_H
#define UI_CENTRALLEFTWIDGET_H

#include <QtWidgets>
#include "models/db/entities_tree_model.h"
#include "models/db/entities_tree_sort_filter_proxy_model.h"

namespace meow {
namespace ui {
namespace main_window {

class DbTree;

class CentralLeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralLeftWidget(
        models::db::EntitiesTreeModel * dbEntitiesTreeModel,
        QWidget * parent = nullptr);

    void selectEntity(meow::db::Entity * entity);

    bool onGlobalRefresh();
private:

    Q_SLOT void showErrorMessage(const QString & message);

    void createMainLayout();

    Q_SLOT void selectedDbEntityChanged(
        const QItemSelection &selected,
        const QItemSelection &deselected);

    QVBoxLayout * _mainLayout;
    DbTree * _dbTree;
    models::db::EntitiesTreeModel * _dbEntitiesTreeModel;
    models::db::EntitiesTreeSortFilterProxyModel _entitiesProxyModel;
};


} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRALLEFTWIDGET_H
