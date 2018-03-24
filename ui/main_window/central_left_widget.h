#ifndef UI_CENTRALLEFTWIDGET_H
#define UI_CENTRALLEFTWIDGET_H

#include <QtWidgets>
#include "models/db/entities_tree_model.h"

namespace meow {
namespace ui {
namespace main_window {

class DbTree;

class CentralLeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralLeftWidget(models::db::EntitiesTreeModel * dbEntitiesTreeModel,
                               QWidget * parent = 0);

    void selectEntity(meow::db::Entity * entity);
private:

    void createMainLayout();
    Q_SLOT void selectedDbEntityChanged(
        const QItemSelection &selected,
        const QItemSelection &deselected);


    QVBoxLayout * _mainLayout;
    DbTree * _dbTree;
    models::db::EntitiesTreeModel * _dbEntitiesTreeModel;
};


} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRALLEFTWIDGET_H
