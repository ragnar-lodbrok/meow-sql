#ifndef UI_CENTRAL_RIGHT_ROUTINE_INFO_PARAMETERS_TAB_H
#define UI_CENTRAL_RIGHT_ROUTINE_INFO_PARAMETERS_TAB_H

#include <QtWidgets>

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

class ParametersTab : public QWidget
{
public:
    ParametersTab(models::forms::RoutineForm * form,
                  QWidget *parent = nullptr);
private:

    void createWidgets();

    models::forms::RoutineForm * _form;
    QTableView * _paramsTable;
};

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_INFO_PARAMETERS_TAB_H
