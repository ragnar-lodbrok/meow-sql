#include "actions.h"
#include <QObject>

namespace meow {

Actions::Actions() : QObject()
{
    createActions();
}

void Actions::createActions()
{
    // TODO: "parent" actions to main window so setStatusTip works

    // -------------------------------------------------------------------------
    _disconnect = new QAction(QIcon(":/icons/disconnect.png"),
                              tr("Disconnect"), this);

    _disconnect->setStatusTip(tr("Close selected database connection"));

    // -------------------------------------------------------------------------
    _sessionManager = new QAction(QIcon(":/icons/server_connect.png"),
                                  tr("Session manager"), this);

    _sessionManager->setStatusTip(tr("Display session manager"));

    // -------------------------------------------------------------------------

    _globalRefresh = new QAction(QIcon(":/icons/arrow_refresh.png"),
                               tr("Refresh"), this);
    _globalRefresh->setShortcuts(QKeySequence::Refresh);

    // -------------------------------------------------------------------------
    // ------------------------------ data -------------------------------------
    _dataInsert = new QAction(QIcon(":/icons/add.png"),
                              tr("&Insert row"), this);

    _dataInsert->setStatusTip(tr("Insert row into table"));

    // -------------------------------------------------------------------------
    _dataDelete = new QAction(QIcon(":/icons/delete.png"),
                              tr("&Delete selected row(s)"), this);

    _dataDelete->setStatusTip(tr("Delete selected row(s)"));

    // -------------------------------------------------------------------------
    _dataPostChanges = new QAction(QIcon(":/icons/tick.png"),
                              tr("P&ost"), this);

    // -------------------------------------------------------------------------
    _dataCancelChanges = new QAction(QIcon(":/icons/cross.png"),
                              tr("Cancel editing"), this);

    // -------------------------------------------------------------------------
    _dataSetNULL = new QAction(QIcon(":/icons/page_white_delete.png"),
                               tr("NULL"), this);
    _dataSetNULL->setStatusTip(tr("Set focused cell to NULL"));

    // -------------------------------------------------------------------------

    _dataRefresh = new QAction(QIcon(":/icons/arrow_refresh.png"),
                               tr("Refresh"), this);
    _dataRefresh->setShortcuts(QKeySequence::Refresh);

    // -------------------------------------------------------------------------
    _logClear = new QAction(tr("Clear"), this);
    _logClear->setStatusTip(tr("Clear query log"));

    // -------------------------------------------------------------------------

    _exportDatabase = new QAction(QIcon(":/icons/database_save.png"),
                              tr("Export database as SQL"), this);
    _exportDatabase->setStatusTip(tr("Dump database objects to an SQL file"));

}

} // namespace meow
