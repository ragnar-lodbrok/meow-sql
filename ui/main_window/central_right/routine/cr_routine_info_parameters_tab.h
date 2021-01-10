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

class RoutineParametersTools;

class ParametersTab : public QWidget
{
public:
    ParametersTab(models::forms::RoutineForm * form,
                  QWidget *parent = nullptr);

    void refreshData();
    void validateControls();

    Q_SLOT void onAddAction();
    Q_SLOT void onRemoveAction();
    Q_SLOT void onClearAction();
    Q_SLOT void onMoveUpAction();
    Q_SLOT void onMoveDownAction();

private:

    void createWidgets();

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    void scrollToCurrent();

    models::forms::RoutineForm * _form;

    RoutineParametersTools * _tools;
    QTableView * _paramsTable;
};

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_INFO_PARAMETERS_TAB_H
