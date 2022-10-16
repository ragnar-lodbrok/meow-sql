#include "output_format_widget.h"
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {
namespace export_query {

OutputFormatWidget::OutputFormatWidget(
        presenters::ExportQueryPresenter * presenter,
        QWidget *parent)
    : QWidget(parent)
    , _presenter(presenter)
{
    setMinimumSize(20, 40);

    createWidgets();
    fillDataFromPresenter();
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
    _formatCombobox->setMaxVisibleItems(20);

    mainGroupLayout->addWidget(_formatCombobox, 0, Qt::AlignTop);

    _groupBox->setLayout(mainGroupLayout);
}

void OutputFormatWidget::fillDataFromPresenter()
{
    _formatCombobox->blockSignals(true);
    _formatCombobox->clear();
    _formatCombobox->addItems(_presenter->formatNames());
    //_formatCombobox->setCurrentText(_presenter->formatName());
    _formatCombobox->blockSignals(false);
}

} // namespace export_query
} // namespace ui
} // namespace meow
