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

    connect(_presenter, &ui::presenters::ExportQueryPresenter::formatChanged,
            this, &OutputFormatWidget::onPresenterFormatChanged);
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
    connect(_formatCombobox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &OutputFormatWidget::onFormatComboboxIndexChanged);


    mainGroupLayout->addWidget(_formatCombobox, 0, Qt::AlignTop);

    _groupBox->setLayout(mainGroupLayout);
}

void OutputFormatWidget::fillDataFromPresenter()
{
    _formatCombobox->blockSignals(true);
    _formatCombobox->clear();
    QMap<QString, QString> formats = _presenter->formatNames();
    auto i = formats.constBegin();
    while (i != formats.constEnd()) {
        _formatCombobox->addItem(i.value(), i.key());
        ++i;
    }

    _formatCombobox->setCurrentIndex(_formatCombobox->findData(
                                         _presenter->formatId()));
    _formatCombobox->blockSignals(false);
}

void OutputFormatWidget::onFormatComboboxIndexChanged(int index)
{
    _presenter->setFormatId(_formatCombobox->itemData(index).toString());
}

void OutputFormatWidget::onPresenterFormatChanged()
{
    _formatCombobox->setCurrentIndex(_formatCombobox->findData(
                                         _presenter->formatId()));
}

} // namespace export_query
} // namespace ui
} // namespace meow
