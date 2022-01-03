#ifndef UI_CENTRALLEFTWIDGET_H
#define UI_CENTRALLEFTWIDGET_H

#include <QtWidgets>
#include "ui/models/entities_tree_model.h"
#include "ui/models/entities_tree_sort_filter_proxy_model.h"

namespace meow {
namespace ui {
namespace main_window {

class DbTree;

class CentralLeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralLeftWidget(
        models::EntitiesTreeModel * dbEntitiesTreeModel,
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
    QLineEdit * _databaseFilterEdit;
    QLineEdit * _tableFilterEdit;
    DbTree * _dbTree;
    models::EntitiesTreeModel * _dbEntitiesTreeModel;
    models::EntitiesTreeSortFilterProxyModel _entitiesProxyModel;
};


} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRALLEFTWIDGET_H
