#ifndef UI_MAINWINDOW_CENTRALWIDGET_H
#define UI_MAINWINDOW_CENTRALWIDGET_H

#include <QtWidgets>
#include "central_left_widget.h"
#include "central_right_widget.h"
#include "central_bottom_widget.h"
#include "ui/models/entities_tree_model.h"

namespace meow {
namespace ui {
namespace main_window {

class CentralWidget : public QWidget
{
public:
    explicit CentralWidget(models::EntitiesTreeModel * dbEntitiesTreeModel,
                           QWidget * parent = nullptr);

    ~CentralWidget();

    void setActiveDBEntity(const db::EntityPtr & entity, bool select = false);
    void onCreatingNewEntity(const db::EntityPtr & entity);

    void onGlobalRefresh();
private:

    Q_SLOT void toggleBottomWidget(bool show);

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

    models::EntitiesTreeModel * _dbEntitiesTreeModel;
};

} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_MAINWINDOW_CENTRALWIDGET_H
