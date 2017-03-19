#ifndef UI_CENTRALRIGHTWIDGET_H
#define UI_CENTRALRIGHTWIDGET_H

#include <QtWidgets>
#include "models/ui/central_right_widget_model.h"
#include "central_right/host/central_right_host_tab.h"

namespace meow {
namespace ui {
namespace main_window {

class CentralRightWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralRightWidget(QWidget *parent = 0);

    void setActiveDBEntity(db::Entity * entity);

private:

    void createRootTabs();

    central_right::HostTab * hostTab();

    models::ui::CentralRightWidgetModel _model;

    QTabWidget  * _rootTabs;
    central_right::HostTab * _hostTab;

};


} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRALRIGHTWIDGET_H
