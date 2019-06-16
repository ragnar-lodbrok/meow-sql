#include "bottom_widget.h"

namespace meow {
namespace ui {
namespace export_database {

BottomWidget::BottomWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

void BottomWidget::createWidgets()
{
    auto layout = new QHBoxLayout();
    this->setLayout(layout);

    layout->addStretch(1);

    _exportButton = new QPushButton(tr("Export"));
    layout->addWidget(_exportButton);

    _cancelButton = new QPushButton(tr("Cancel"));
    layout->addWidget(_cancelButton);
}

} // namespace export_database
} // namespace ui
} // namespace meow
