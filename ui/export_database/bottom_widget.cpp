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
    /*connect(_exportButton,
            &QAbstractButton::clicked,
            this,
            &Window::openCurrentSession
    );*/

    _cancelButton = new QPushButton(tr("Cancel"));
    layout->addWidget(_cancelButton);
    /*connect(_cancelButton,
            &QAbstractButton::clicked,
            this,
            &Window::reject
    );*/
}

} // namespace export_database
} // namespace ui
} // namespace meow
