#include "row_selection_widget.h"

namespace meow {
namespace ui {
namespace export_query {

RowSelectionWidget::RowSelectionWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(20, 40);

    createWidgets();
}

void RowSelectionWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _groupBox = new QGroupBox(tr("Row selection"));
    _groupBox->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(_groupBox);

    QVBoxLayout * mainGroupLayout = new QVBoxLayout();

    _selectionRadioButton = new QRadioButton(tr("Selection"));
    _completeRadioButton = new QRadioButton(tr("Complete"));

    mainGroupLayout->addWidget(_selectionRadioButton);
    mainGroupLayout->addWidget(_completeRadioButton);
    mainGroupLayout->addStretch(1);

    _groupBox->setLayout(mainGroupLayout);
}

} // namespace export_query
} // namespace ui
} // namespace meow
