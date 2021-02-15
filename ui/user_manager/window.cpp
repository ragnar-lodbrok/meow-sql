#include "window.h"
#include "left_widget.h"
#include "right_widget.h"
#include "ui/main_window/main_window.h"

namespace meow {
namespace ui {
namespace user_manager {

Window::Window(main_window::Window * mainWindow, db::SessionEntity * session)
    : QDialog(mainWindow, Qt::WindowCloseButtonHint)
    , _mainWindow(mainWindow)
    , _session(session)
{

    createWidgets();

    setMinimumSize(QSize(800, 400));
    setWindowTitle(tr("User manager"));

    //loadGeometryFromSettings(); // TODO
}

Window::~Window()
{
    //saveGeometryToSettings(); // TODO
}

void Window::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _splitter = new QSplitter(this);
    _splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_splitter);

    _leftWidget = new LeftWidget(_session);
    _leftWidget->setMinimumSize(QSize(300, 400));

    _rightWidget = new RightWidget();
    _rightWidget->setMinimumSize(QSize(470, 400));

    _splitter->addWidget(_leftWidget);
    _splitter->setStretchFactor(0, 1);
    _splitter->addWidget(_rightWidget);
    _splitter->setStretchFactor(1, 2);
}

} // namespace user_manager
} // namespace ui
} // namespace
