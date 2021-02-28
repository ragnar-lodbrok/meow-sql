#ifndef UI_USER_MANAGER_CREDENTIALS_TAB_H
#define UI_USER_MANAGER_CREDENTIALS_TAB_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models

namespace ui {
namespace user_manager {

class CredentialsTab : public QWidget
{
public:
    explicit CredentialsTab(models::forms::UserManagementForm * form,
                            QWidget * parent = nullptr);
private:

    void createWidgets();

    QLabel * _usernameLabel;
    QLineEdit * _usernameEdit;

    QLabel * _hostLabel;
    QLineEdit * _hosteEdit;

    models::forms::UserManagementForm * _form;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_CREDENTIALS_TAB_H
