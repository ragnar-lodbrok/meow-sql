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
}

} // namespace meow
