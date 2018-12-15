#ifndef UI_MAINWINDOW_CENTRALWIDGET_H
#define UI_MAINWINDOW_CENTRALWIDGET_H

#include <QtWidgets>
#include "central_left_widget.h"
#include "central_right_widget.h"
#include "central_bottom_widget.h"
#include "models/db/entities_tree_model.h"

namespace meow {
namespace ui {
namespace main_window {

class CentralWidget : public QWidget
{
public:
    explicit CentralWidget(models::db::EntitiesTreeModel * dbEntitiesTreeModel,
                           QWidget * parent = nullptr);

    ~CentralWidget();

    void setActiveDBEntity(db::Entity * entity, bool select = false);
    void onCreatingNewEntity(db::Entity * entity);
private:

    void saveGeometryToSettings();
    void loadGeometryFromSettings();

    void createMainLayout();
    void createCenterLayout();

    QVBoxLayout * _mainLayout;
    QSplitter * _mainVerticalSplitter;

    QHBoxLayout * _mainHorizLayout;
    QSplitter * _mainHorizontalSplitter;

    CentralLeftWidget * _mainLeftWidget;
    CentralRightWidget * _mainRightWidget;
    CentralBottomWidget * _mainBottomWidget;

    models::db::EntitiesTreeModel * _dbEntitiesTreeModel;
};

} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_MAINWINDOW_CENTRALWIDGET_H
