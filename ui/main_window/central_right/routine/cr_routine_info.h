#ifndef UI_CENTRAL_RIGHT_ROUTINE_INFO_H
#define UI_CENTRAL_RIGHT_ROUTINE_INFO_H

#include <QtWidgets>
#include "ui/presenters/routine_info_widget_model.h"

namespace meow {
namespace ui {

namespace presenters {
    class RoutineForm;
}

namespace main_window {
namespace central_right {

namespace routine_info {

class OptionsTab;
class ParametersTab;

}

class RoutineInfo : public QWidget
{
public:
    explicit RoutineInfo(presenters::RoutineForm * form,
                         QWidget *parent = nullptr);

    void refreshData();
    
    void validateControls();

    void onBeforeEntityEditing();

private:

    void createTabs();

    presenters::RoutineInfoWidgetModel _tabsModel;
    presenters::RoutineForm * _form;

    QTabWidget  * _rootTabs;
    routine_info::OptionsTab * _optionsTab;
    routine_info::ParametersTab * _parametersTab;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_INFO_H
