#ifndef UI_CENTRALRIGHTHOSTTAB_H
#define UI_CENTRALRIGHTHOSTTAB_H

#include <QtWidgets>
#include "cr_host_databases_tab.h"
#include "models/ui/central_right_host_widget_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class HostTab : public QWidget
{
    Q_OBJECT
public:
    explicit HostTab(QWidget *parent = 0);
    void setCurrentEntity(meow::db::SessionEntity * curEntity);

    enum Tabs {
        Databases,
        Variables
    };

private:

    void createRootTabs();
    void onSessionChanged();

    QTabWidget  * _rootTabs;
    HostDatabasesTab * _databasesTab;

    models::ui::CentralRightHostWidgetModel _model;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRALRIGHTHOSTTAB_H
