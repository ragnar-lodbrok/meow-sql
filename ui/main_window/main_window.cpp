#include "main_window.h"
#include <QMenuBar>
#include <QDebug>
#include "ui/session_manager/window.h"
#include "app.h"
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
    setWindowTitle("MeowSQL");
    setWindowIcon(QIcon(":/icons/logo.png"));
    resize(QSize(880, 500));

    //createMenus(); // no actions
    _centralWidget = new CentralWidget(&_dbEntitiesTreeModel);
    setCentralWidget(_centralWidget);

    //statusBar(); // hide as we have nothing to show

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::activeEntityChanged,
            this,
            &Window::activeDBEntityChanged);

    connect(meow::app()->dbConnectionsManager(),
            &meow::db::ConnectionsManager::creatingNewEntity,
            [=](db::Entity * entity) {
                _centralWidget->onCreatingNewEntity(entity);
            });

}

Window::~Window()
{

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

    qDebug() << "Opening " << params.sessionName();

    try {

        meow::app()->dbConnectionsManager()->openDBConnection(params);

    } catch(meow::db::Exception & ex) {

        qDebug() << "Opening failed: " << ex.message();

        showErrorMessage(ex.message());

        return false;
    }

    qDebug() << "Opened " << params.sessionName();

    return true;
}

// private: --------------------------------------------------------------------


void Window::sessionManagerDialogCanceled()
{
    if (meow::app()->dbConnectionsManager()->isNoOpenedConnections()) {
        QMetaObject::invokeMethod(this, "close", Qt::QueuedConnection);
    }
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

void Window::activeDBEntityChanged(db::Entity * newEntity, bool select)
{
    _centralWidget->setActiveDBEntity(newEntity, select);
}

} // namespace main_window
} // namespace ui
} // namespace meow

