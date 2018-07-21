#include "actions.h"
#include <QObject>

namespace meow {

Actions::Actions() : QObject()
{
    createActions();
}

void Actions::createActions()
{
    // -------------------------------------------------------------------------
    _disconnect = new QAction(QIcon(":/icons/disconnect.png"),
                              tr("Disconnect"), this);

    _disconnect->setStatusTip(tr("Close selected database connection"));

    // -------------------------------------------------------------------------
    _sessionManager = new QAction(QIcon(":/icons/server_connect.png"),
                                  tr("Session manager"), this);

    _sessionManager->setStatusTip(tr("Display session manager"));


}

} // namespace meow
