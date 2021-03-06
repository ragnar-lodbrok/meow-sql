#ifndef UI_USER_MANAGER_PRIVILEGES_WIDGET_H
#define UI_USER_MANAGER_PRIVILEGES_WIDGET_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models

namespace ui {
namespace user_manager {

class PrivilegesWidget : public QWidget
{
public:
    explicit PrivilegesWidget(models::forms::UserManagementForm * form,
                              QWidget * parent = nullptr);
    void fillDataFromForm();
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_PRIVILEGES_WIDGET_H
