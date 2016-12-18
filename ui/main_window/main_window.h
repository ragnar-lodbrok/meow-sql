#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QMainWindow>
#include "central_widget.h"

namespace meow {
namespace ui {
namespace main_window {

class Window : public QMainWindow
{
    //Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();
private:
    void createMenus();
    CentralWidget * _centralWidget;
};

} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_MAINWINDOW_H
