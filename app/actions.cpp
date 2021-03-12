#include "actions.h"
#include "app.h"
#include <QKeySequence>
#include <QObject>
#include <QDebug>

namespace meow {

Actions::Actions(App * app) : QObject()
{
    createActions(app);
}

void Actions::createActions(App * app)
{

    settings::Geometry * geometrySettings = app->settings()->geometrySettings();

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
    _userManager = new QAction(QIcon(":/icons/group.png"),
                                  tr("User manager"), this);

    _userManager->setStatusTip(tr("Manage user authentication and privileges"));

    // -------------------------------------------------------------------------

    _globalRefresh = new QAction(QIcon(":/icons/arrow_refresh.png"),
                               tr("Refresh"), this);
    _globalRefresh->setShortcut(QKeySequence::Refresh);

    // -------------------------------------------------------------------------

    _showGlobalFilterPanel = new QAction(QIcon(":/icons/find.png"),
                               tr("Filter panel"), this);
    _showGlobalFilterPanel->setShortcut(
                QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_F));
    _showGlobalFilterPanel->setCheckable(true);
    _showGlobalFilterPanel->setChecked(geometrySettings->showFilterPanel());

    connect(_showGlobalFilterPanel, &QAction::changed, [=](){
        geometrySettings->setShowFilterPanel(
                    _showGlobalFilterPanel->isChecked());
    });

    connect(geometrySettings, &settings::Geometry::showFilterPanelChanged,
            [=](bool show){
                if (_showGlobalFilterPanel->isChecked() != show) {
                    _showGlobalFilterPanel->setChecked(show);
                }
            }
    );

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
    // No F5 shortcut, conflicts with global refresh:
    //_dataRefresh->setShortcuts(QKeySequence::Refresh);

    // -------------------------------------------------------------------------
    _logClear = new QAction(tr("Clear"), this);
    _logClear->setStatusTip(tr("Clear query log"));

    // -------------------------------------------------------------------------

    _exportDatabase = new QAction(QIcon(":/icons/database_save.png"),
                              tr("Export database as SQL"), this);
    _exportDatabase->setStatusTip(tr("Dump database objects to an SQL file"));

}

} // namespace meow
