#include "row_selection_widget.h"
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {
namespace export_query {

RowSelectionWidget::RowSelectionWidget(
        presenters::ExportQueryPresenter *presenter,
        QWidget *parent)
    : QWidget(parent)
    , _presenter(presenter)
{
    setMinimumSize(20, 40);

    createWidgets();
    fillDataFromPresenter();
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

    _selectionRadioButton = new QRadioButton();
    _completeRadioButton = new QRadioButton();

    connect(_selectionRadioButton, &QAbstractButton::toggled,
            this, &RowSelectionWidget::onSelectionRadioButtonToggled);
    connect(_completeRadioButton, &QAbstractButton::toggled,
            this, &RowSelectionWidget::onSelectionRadioButtonToggled);

    mainGroupLayout->addWidget(_selectionRadioButton);
    mainGroupLayout->addWidget(_completeRadioButton);
    mainGroupLayout->addStretch(1);

    _groupBox->setLayout(mainGroupLayout);
}

void RowSelectionWidget::fillDataFromPresenter()
{
    if (_presenter->isSelectedRows()) {
        _selectionRadioButton->blockSignals(true);
        _selectionRadioButton->setChecked(true);
        _selectionRadioButton->blockSignals(false);
    } else {
        _completeRadioButton->blockSignals(true);
        _completeRadioButton->setChecked(true);
        _completeRadioButton->blockSignals(false);
    }

    _selectionRadioButton->setText(tr("Selection")
                                   + " " + _presenter->selectedRowsStats());

    _completeRadioButton->setText(tr("Complete")
                                   + " " + _presenter->allRowsStats());
}

void RowSelectionWidget::onSelectionRadioButtonToggled(bool checked)
{
    if (checked) {
        _presenter->setOnlySelectedRows(
            (sender() == _selectionRadioButton)
        );
    }
}

} // namespace export_query
} // namespace ui
} // namespace meow
