#ifndef UI_SESSIONMANAGER_SESSIONFORM_H
#define UI_SESSIONMANAGER_SESSIONFORM_H

#include <QWidget>

#include "start_tab.h"
#include "settings_tab.h"
#include "ssh_tunnel_tab.h"

#include "ui/presenters/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class SessionForm : public QWidget
{
    Q_OBJECT
public:
    explicit SessionForm(QWidget *parent = nullptr);

    void setConnectionParamsForm(presenters::ConnectionParametersForm * form);
private:

    void createDetailsTabs();
    Q_SLOT void fillDataFromForm();

    SSHTunnelTab * sshTunnelTab();

    QTabWidget  * _detailsTabs;
    StartTab * _startTab;
    SettingsTab * _settingsTab;
    SSHTunnelTab * _sshTunnelTab;

    presenters::ConnectionParametersForm * _connectionParamsForm;

    bool _startTabIsHidden;
    bool _settingsTabIsHidden;
    bool _sshTunnelTabIsHidden;
};

} // namespace session_manager
} // namespace ui
} // namespace meow

#endif // UI_SESSIONMANAGER_SESSIONFORM_H
