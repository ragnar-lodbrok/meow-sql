#ifndef UI_USER_MANAGER_RIGHT_WIDGET_H
#define UI_USER_MANAGER_RIGHT_WIDGET_H

#include <QWidget>
#include "options_widget.h"
#include "privileges_widget.h"

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models


namespace ui {
namespace user_manager {

class Window;

class RightWidget : public QWidget
{
public:
    explicit RightWidget(models::forms::UserManagementForm * form,
                         Window * window);
    void fillDataFromForm();
    void validateControls();

    void onAddUserClicked();

private:

    void createWidgets(models::forms::UserManagementForm * form,
                       Window * window);

    OptionsWidget * _options;
    PrivilegesWidget * _privileges;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_RIGHT_WIDGET_H
