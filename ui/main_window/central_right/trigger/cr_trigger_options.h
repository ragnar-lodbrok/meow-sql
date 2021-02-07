#ifndef UI_CENTRAL_RIGHT_TRIGGER_OPTIONS_H
#define UI_CENTRAL_RIGHT_TRIGGER_OPTIONS_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class TriggerForm;
}
}

namespace ui {
namespace main_window {
namespace central_right {

class TriggerOptions : public QWidget
{
public:
    TriggerOptions(models::forms::TriggerForm * form,
                   QWidget *parent = nullptr);

    void refreshData();
    void validateControls();

private:

    void createWidgets();
    void fillDataFromForm();

    models::forms::TriggerForm * _form;

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _definerLabel;
    QComboBox * _definerCombobox;

    QLabel * _tableLabel;
    QComboBox * _tableCombobox;

    QLabel * _eventLabel;
    QComboBox * _actionTimeCombobox;
    QComboBox * _eventCombobox;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow


#endif // UI_CENTRAL_RIGHT_TRIGGER_OPTIONS_H
