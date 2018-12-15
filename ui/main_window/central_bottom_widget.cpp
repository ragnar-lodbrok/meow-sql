#include "central_bottom_widget.h"
#include "central_log_widget.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {

CentralBottomWidget::CentralBottomWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

CentralBottomWidget::~CentralBottomWidget()
{
    meow::app()->log()->removeSink(_logWidget);
}

void CentralBottomWidget::createWidgets()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _logWidget = new CentralLogWidget();
    _mainLayout->addWidget(_logWidget);

    meow::app()->log()->addSink(_logWidget);
}

} // namespace meow
} // namespace ui
} // namespace main_window
