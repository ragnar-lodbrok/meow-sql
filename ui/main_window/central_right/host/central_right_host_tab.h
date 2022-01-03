#ifndef UI_CENTRAL_RIGHT_HOST_TAB_H
#define UI_CENTRAL_RIGHT_HOST_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "cr_host_databases_tab.h"
#include "cr_host_variables_tab.h"
#include "ui/presenters/central_right_host_widget_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class HostTab : public BaseRootTab
{
    Q_OBJECT
public:
    explicit HostTab(QWidget *parent = nullptr);
    void setCurrentEntity(meow::db::SessionEntity * curEntity);

    enum class Tabs {
        Databases,
        Variables
    };

    static void showErrorMessage(const QString& message);

    void onGlobalRefresh();

private:

    void createRootTabs();
    HostVariablesTab * variablesTab();
    bool removeVariablesTab();
    bool removeTab(QWidget * tab);

    void onSessionChanged(meow::db::SessionEntity * session);

    Q_SLOT void rootTabChanged(int index);

    QTabWidget  * _rootTabs;
    HostDatabasesTab * _databasesTab;
    HostVariablesTab * _variablesTab;

    presenters::CentralRightHostWidgetModel _model;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_HOST_TAB_H
