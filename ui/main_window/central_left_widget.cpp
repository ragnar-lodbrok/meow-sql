#include "central_left_widget.h"

namespace meow {
namespace ui {
namespace main_window {

CentralLeftWidget::CentralLeftWidget(QWidget *parent) : QWidget(parent)
{
    createMainLayout();
}

void CentralLeftWidget::createMainLayout()
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _dbTree = new QTreeView();
    _mainLayout->addWidget(_dbTree);
}

} // namespace meow
} // namespace ui
} // namespace main_window
