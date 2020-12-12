#ifndef UI_CENTRAL_RIGHT_ROUTINE_INFO_H
#define UI_CENTRAL_RIGHT_ROUTINE_INFO_H

#include <QtWidgets>
#include "models/ui/routine_info_widget_model.h"

namespace meow {

namespace models {
namespace forms {
    class RoutineForm;
}
}

namespace ui {
namespace main_window {
namespace central_right {

namespace routine_info {

class OptionsTab;
class ParametersTab;

}

class RoutineInfo : public QWidget
{
public:
    explicit RoutineInfo(models::forms::RoutineForm * form,
                         QWidget *parent = nullptr);

    void refreshData();
    
private:

    void createTabs();

    models::ui::RoutineInfoWidgetModel _tabsModel;
    models::forms::RoutineForm * _form;

    QTabWidget  * _rootTabs;
    routine_info::OptionsTab * _optionsTab;
    routine_info::ParametersTab * _parametersTab;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_INFO_H
