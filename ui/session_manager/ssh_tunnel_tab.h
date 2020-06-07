#ifndef UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H
#define UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H

#include <QtWidgets>

#include "models/forms/connection_parameters_form.h"

namespace meow {
namespace ui {
namespace session_manager {

class SSHTunnelTab : public QWidget
{
public:
    explicit SSHTunnelTab(QWidget *parent = nullptr);
};

} // namespace session_manager
} // namespace ui
} // namespace meow


#endif // UI_SESSIONMANAGER_SSH_TUNNEL_TAB_H
