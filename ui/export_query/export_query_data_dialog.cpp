#include "export_query_data_dialog.h"
#include "output_target_widget.h"
#include "output_format_widget.h"
#include "row_selection_widget.h"
#include "options_widget.h"

namespace meow {
namespace ui {
namespace export_query {

Dialog::Dialog()
{
    setMinimumSize(400, 350);
    setWindowTitle(tr("Export grid rows"));

    createWidgets();

    resize(700, 500);

    connect(&_presenter, &ui::presenters::ExportQueryPresenter::formatChanged,
            this, &Dialog::onFormatChanged);
}

void Dialog::setData(
        models::BaseDataTableModel * model,
        QItemSelectionModel * selection)
{
    _presenter.setData(model, selection);
    _rowSelectionWidget->fillDataFromPresenter();
}

void Dialog::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    _outputTargetWidget = new OutputTargetWidget(&_presenter);
    mainLayout->addWidget(_outputTargetWidget, 0, Qt::AlignTop);

    QHBoxLayout * horizLayout = new QHBoxLayout();

    _outputFormatWidget = new OutputFormatWidget(&_presenter);
    horizLayout->addWidget(_outputFormatWidget, 0);

    _rowSelectionWidget = new RowSelectionWidget(&_presenter);
    horizLayout->addWidget(_rowSelectionWidget, 0);

    mainLayout->addLayout(horizLayout, 0);

    _optionsWidget = new OptionsWidget(&_presenter);
    mainLayout->addWidget(_optionsWidget, 0, Qt::AlignTop);

    mainLayout->addStretch(1);

    _buttonBox = new QDialogButtonBox();

    _okButton = _buttonBox->addButton(QDialogButtonBox::Ok);
    _cancelButton = _buttonBox->addButton(QDialogButtonBox::Cancel);

    connect(_buttonBox, &QDialogButtonBox::accepted,
            this, &Dialog::onAccept);
    connect(_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(_buttonBox, 0, Qt::AlignRight);
}

void Dialog::onAccept()
{
    _presenter.run();
}

void Dialog::onFormatChanged()
{
    _optionsWidget->fillDataFromPresenter();
}

} // namespace export_query
} // namespace ui
} // namespace meow