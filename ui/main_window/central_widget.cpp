#include "central_widget.h"

namespace meow {
namespace ui {
namespace main_window {

CentralWidget::CentralWidget(QWidget * parent)
    :QWidget(parent)
{
    createMainLayout();
}

void CentralWidget::createMainLayout()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _mainHorizontalSplitter = new QSplitter(this);
    _mainHorizontalSplitter->setChildrenCollapsible(false);
    _mainLayout->addWidget(_mainHorizontalSplitter);

    _mainLeftWidget = new CentralLeftWidget();
    _mainLeftWidget->setMinimumSize(QSize(200, 400));

    _mainRightWidget = new CentralRightWidget();
    _mainRightWidget->setMinimumSize(QSize(350, 400));

    _mainHorizontalSplitter->addWidget(_mainLeftWidget);
    //_mainHorizontalSplitter->setStretchFactor(0, 1);
    _mainHorizontalSplitter->addWidget(_mainRightWidget);
    //_mainHorizontalSplitter->setStretchFactor(1, 2);
}

} // namespace main_window
} // namespace ui
} // namespace meow
