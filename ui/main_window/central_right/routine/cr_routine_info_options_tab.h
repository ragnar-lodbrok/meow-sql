#ifndef UI_CENTRAL_RIGHT_ROUTINE_INFO_OPTIONS_TAB_H
#define UI_CENTRAL_RIGHT_ROUTINE_INFO_OPTIONS_TAB_H

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

class OptionsTab : public QWidget
{
public:
    explicit OptionsTab(models::forms::RoutineForm * form,
                        QWidget *parent = nullptr);
    void refreshData();
private:
    void createWidgets();
    void fillDataFromForm();
    void validateControls();

    models::forms::RoutineForm * _form;

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _definerLabel;
    QComboBox * _definerCombobox;

    QLabel * _commentLabel;
    QLineEdit * _commentEdit;

    QLabel * _routineTypeLabel;
    QComboBox * _routineTypeCombobox;

    QLabel * _dataAccessLabel;
    QComboBox * _dataAccessCombobox;

    QLabel * _returnTypeLabel;
    QComboBox * _returnTypeCombobox;

    QLabel * _sqlSecurityLabel;
    QComboBox * _sqlSecurityCombobox;

    QCheckBox * _deterministicChecbox;
};

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_INFO_OPTIONS_TAB_H
