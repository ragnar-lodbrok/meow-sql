#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>
#include "central_widget.h"
#include "db/connection_parameters.h"

namespace meow {
namespace ui {
namespace main_window {

class Window : public QMainWindow
{
    //Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();
    void showSessionManagerDialog();
    bool openDBConnection(db::ConnectionParameters & params);
private:
    void createMenus();

    void showErrorMessage(const QString& message);

    CentralWidget * _centralWidget;
};

} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_MAINWINDOW_H
