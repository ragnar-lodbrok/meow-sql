#include "main_window.h"
#include <QMenuBar>
#include "helpers/logger.h"
#include "ui/session_manager/window.h"
#include "ui/user_manager/window.h"
#include "app/app.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {

// public: ---------------------------------------------------------------------

Window::Window(QWidget *parent)
    : QMainWindow(parent),
      _dbEntitiesTreeModel(meow::app()->dbConnectionsManager())
{
    setMinimumSize(QSize(700, 400));
    setWindowTitle(getWindowTitle());
    setWindowIcon(QIcon(":/icons/logo.png"));

    restoreGeometryAndState();

    //createMenus(); // no actions
    _centralWidget = new CentralWidget(&_dbEntitiesTreeModel);
    setCentralWidget(_centralWidget);

    _statusBar = new StatusBar(this);
    setStatusBar(_statusBar);

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::activeEntityChanged,
            this,
            &Window::activeDBEntityChanged);

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::creatingNewEntity,
            [=](db::Entity * entity) {
                _centralWidget->onCreatingNewEntity(entity);
            });

    connect(meow::app()->actions()->disconnect(),
            &QAction::triggered,
            this,
            &Window::onDisconnectAction);

    connect(meow::app()->actions()->sessionManager(),
            &QAction::triggered,
            this,
            &Window::onSessionManagerAction);

    connect(meow::app()->actions()->userManager(),
            &QAction::triggered,
            this,
            &Window::onUserManagerAction);

    connect(meow::app()->actions()->globalRefresh(),
            &QAction::triggered,
            this,
            &Window::onGlobalRefresh);

    // add hotkeys:
    this->addAction(meow::app()->actions()->globalRefresh());
    this->addAction(meow::app()->actions()->showGlobalFilterPanel());
}

void Window::showSessionManagerDialog()
{
    meow::ui::session_manager::Window sessionManagerWindow(this);
    connect(&sessionManagerWindow,
            &meow::ui::session_manager::Window::rejected,
            this,
            &Window::sessionManagerDialogCanceled
    );
    sessionManagerWindow.exec();
}

bool Window::openDBConnection(db::ConnectionParameters & params)
{

    meowLogDebug() << "Opening " << params.sessionName();

    try {

        meow::app()->dbConnectionsManager()->openDBConnection(params);

    } catch(meow::db::Exception & ex) {

        //meowLogDebug() << "Opening failed: " << ex.message();

        showErrorMessage(ex.message());

        return false;
    }

    meowLogDebug() << "Opened " << params.sessionName();

    return true;
}

// protected: ------------------------------------------------------------------

void Window::closeEvent(QCloseEvent *event)
{
    writeGeometryAndState();
    QMainWindow::closeEvent(event);
}

// private: --------------------------------------------------------------------


void Window::sessionManagerDialogCanceled()
{
    if (meow::app()->dbConnectionsManager()->isNoOpenedConnections()) {
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
    }
}

void Window::onDisconnectAction(bool checked)
{
    Q_UNUSED(checked);
    try {
        meow::app()->dbConnectionsManager()->closeActiveConnection();
    } catch(meow::db::Exception & ex) {
        showErrorMessage(ex.message());
    }

    if (meow::app()->dbConnectionsManager()->isNoOpenedConnections()) {
        showSessionManagerDialog();
    }
}

void Window::onSessionManagerAction(bool checked)
{
    Q_UNUSED(checked);
    showSessionManagerDialog();
}

void Window::onUserManagerAction()
{
    meow::db::SessionEntity * currentSession
            = meow::app()->dbConnectionsManager()->activeSession();

    Q_ASSERT(currentSession != nullptr);
    if (!currentSession) return;

    meow::ui::user_manager::Window userManagerWindow(this, currentSession);
    userManagerWindow.exec();
}

void Window::onGlobalRefresh(bool checked)
{
    Q_UNUSED(checked);
    _centralWidget->onGlobalRefresh();
}

void Window::createMenus()
{
    menuBar()->addMenu(tr("File"));
    menuBar()->addMenu(tr("Edit"));
    menuBar()->addMenu(tr("Search"));
    menuBar()->addMenu(tr("Tools"));
    menuBar()->addMenu(tr("Help"));
}

void Window::showErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

QString Window::getWindowTitle() const
{
    QString path = meow::app()->dbConnectionsManager()->activeEntityPath();
    QString appName = "MeowSQL";

    if (path.isEmpty()) {
        return appName;
    } else {
        return path + " - " + appName;
    }
}

void Window::activeDBEntityChanged(db::Entity * newEntity, bool select)
{
    _centralWidget->setActiveDBEntity(newEntity, select);
    setWindowTitle(getWindowTitle());
}

void Window::writeGeometryAndState()
{
    QSettings settings;
    settings.setValue("ui/main_window/geometry", saveGeometry());
    settings.setValue("ui/main_window/state", saveState());
}

void Window::restoreGeometryAndState()
{
    QSettings settings;

    const QByteArray geometry
        = settings.value("ui/main_window/geometry", QByteArray()).toByteArray();
    if (geometry.isEmpty()) {

        // screen center
        this->setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                QSize(880, 500),
                qApp->desktop()->availableGeometry()
            )
        );
    } else {
        restoreGeometry(geometry);
        const QByteArray state
            = settings.value("ui/main_window/state", QByteArray())
                .toByteArray();
        if (!state.isEmpty()) {
            restoreState(state);
        }
    }
}

} // namespace main_window
} // namespace ui
} // namespace meow

