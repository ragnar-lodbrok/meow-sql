#include "output_format_widget.h"

namespace meow {
namespace ui {
namespace export_query {

OutputFormatWidget::OutputFormatWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(20, 40);

    createWidgets();
}

void OutputFormatWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _groupBox = new QGroupBox(tr("Output format"));
    _groupBox->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(_groupBox);

    QVBoxLayout * mainGroupLayout = new QVBoxLayout();

    _formatCombobox = new QComboBox;

    mainGroupLayout->addWidget(_formatCombobox, 0, Qt::AlignTop);

    _groupBox->setLayout(mainGroupLayout);
}

} // namespace export_query
} // namespace ui
} // namespace meow
