#ifndef UI_SESSIONMANAGER_SESSIONFORM_H
#define UI_SESSIONMANAGER_SESSIONFORM_H

#include <QWidget>

#include "ui/session_manager/start_tab.h"
#include "ui/session_manager/settings_tab.h"

#include "models/forms/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class SessionForm : public QWidget
{
    Q_OBJECT
public:
    explicit SessionForm(QWidget *parent = nullptr);

    void setConnectionParamsForm(models::forms::ConnectionParametersForm * form);
private:

    void createDetailsTabs();
    void fillDataFromForm();

    QTabWidget  * _detailsTabs;
    StartTab * _startTab;
    SettingsTab * _settingsTab;

    models::forms::ConnectionParametersForm * _connectionParamsForm;

    bool _startTabIsHidden;
    bool _settingsTabIsHidden;
};

} // namespace session_manager
} // namespace ui
} // namespace meow

#endif // UI_SESSIONMANAGER_SESSIONFORM_H
