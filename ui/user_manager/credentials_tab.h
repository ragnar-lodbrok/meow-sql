#ifndef UI_USER_MANAGER_CREDENTIALS_TAB_H
#define UI_USER_MANAGER_CREDENTIALS_TAB_H

#include <QtWidgets>

namespace meow {

namespace ui {
namespace presenters {
    class UserManagementForm;
} // namespace presenters

namespace user_manager {

class CredentialsTab : public QWidget
{
    Q_OBJECT
public:
    explicit CredentialsTab(presenters::UserManagementForm * form,
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

    presenters::UserManagementForm * _form;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_CREDENTIALS_TAB_H
