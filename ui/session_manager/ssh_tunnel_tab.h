#ifndef UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H
#define UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H

#include <QtWidgets>

#include "ui/presenters/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class SSHTunnelTab : public QWidget
{
    Q_OBJECT // for tr()
public:

    explicit SSHTunnelTab(QWidget *parent = nullptr);

    void setForm(presenters::ConnectionParametersForm * form);

private:

    void createWidgets();
    void fillDataFromForm();

    QGridLayout * _mainGridLayout;

    QLabel * _sshHostPortLabel;
    QLineEdit * _sshHostEdit;
    QSpinBox * _sshPortSpinBox;

    QLabel * _usernameLabel;
    QLineEdit * _usernameEdit;

    QLabel * _passwordLabel;
    QLineEdit * _passwordEdit;

    QLabel * _localPortLabel;
    QSpinBox * _localPortSpinBox;

    presenters::ConnectionParametersForm * _form;

};

} // namespace session_manager
} // namespace ui
} // namespace meow


#endif // UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H
