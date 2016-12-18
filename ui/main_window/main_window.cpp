#include "main_window.h"
#include <QMenuBar>

namespace meow {
namespace ui {
namespace main_window {


Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(QSize(600, 400));
    setWindowTitle("MeowSQL");
    resize(QSize(600, 400));

    createMenus();
    _centralWidget = new CentralWidget();
    setCentralWidget(_centralWidget);

    statusBar();
}

Window::~Window()
{
}

void Window::createMenus()
{
    menuBar()->addMenu(tr("File"));
    menuBar()->addMenu(tr("Edit"));
    menuBar()->addMenu(tr("Search"));
    menuBar()->addMenu(tr("Tools"));
    menuBar()->addMenu(tr("Help"));
}

} // namespace main_window
} // namespace ui
} // namespace meow

