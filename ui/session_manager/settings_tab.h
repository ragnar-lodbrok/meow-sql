#ifndef SESSIONMANAGER_SETTINGSTAB_H
#define SESSIONMANAGER_SETTINGSTAB_H

#include <QtWidgets>

#include "models/forms/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class SettingsTab : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsTab(QWidget *parent = nullptr);
    void setForm(models::forms::ConnectionParametersForm * form);
private:

    void fillDataFromForm();
    void onLoginPromptUpdate();
    void onShowDatabaseListAction();

    QGridLayout * _mainGridLayout;

    QLabel * _networkTypeLabel;
    QComboBox * _networkTypeComboBox;

    QLabel * _hostLabel;
    QLineEdit * _hostEdit;

    QCheckBox * _loginPromptCheckBox;

    QLabel * _userLabel;
    QLineEdit * _userEdit;

    QLabel * _passwordLabel;
    QLineEdit * _passwordEdit;

    QLabel * _portLabel;
    QSpinBox * _portSpinBox;

    QLabel * _databasesLabel;
    QLineEdit * _databasesEdit;

    models::forms::ConnectionParametersForm * _form;
};

} // namespace session_manager
} // namespace ui
} // namespace meow

#endif // SESSIONMANAGER_SETTINGSTAB_H
