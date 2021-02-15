#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>
#include "central_widget.h"
#include "main_window_status_bar.h"
#include "db/connection_parameters.h"
#include "models/db/entities_tree_model.h"

namespace meow {
namespace ui {
namespace main_window {

class Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);
    void showSessionManagerDialog();
    bool openDBConnection(db::ConnectionParameters & params);

protected:

    virtual void closeEvent(QCloseEvent *event) override;

private:

    void writeGeometryAndState();
    void restoreGeometryAndState();

    void createMenus();

    void showErrorMessage(const QString& message);

    QString getWindowTitle() const;

    Q_SLOT void activeDBEntityChanged(db::Entity * newEntity, bool select);

    Q_SLOT void sessionManagerDialogCanceled();

    Q_SLOT void onDisconnectAction(bool checked);

    Q_SLOT void onSessionManagerAction(bool checked);

    Q_SLOT void onUserManagerAction();

    Q_SLOT void onGlobalRefresh(bool checked);

    CentralWidget * _centralWidget;
    StatusBar     * _statusBar;

    models::db::EntitiesTreeModel _dbEntitiesTreeModel;
};

} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_MAINWINDOW_H
