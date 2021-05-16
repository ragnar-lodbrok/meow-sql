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

    void fillDataFromForm();
    void onAddUserClicked();
private:

    void createWidgets();

    Q_SLOT void onShowPasswordsAction();

    QLabel * _usernameLabel;
    QLineEdit * _usernameEdit;

    QLabel * _hostLabel;
    QLineEdit * _hostEdit;

    QLabel * _passwordLabel;
    QLineEdit * _passwordEdit;

    QLabel * _repeatPasswordLabel;
    QLineEdit * _repeatPasswordEdit;

    models::forms::UserManagementForm * _form;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_CREDENTIALS_TAB_H
