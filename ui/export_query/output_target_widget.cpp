#include "output_target_widget.h"
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {
namespace export_query {

OutputTargetWidget::OutputTargetWidget(
        presenters::ExportQueryPresenter * presenter, QWidget *parent)
    : QWidget(parent)
    , _presenter(presenter)
{
    setMinimumSize(100, 120);

    createWidgets();
    fillDataFromPresenter();
    validateControls();
}

void OutputTargetWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _groupBox = new QGroupBox(tr("Output target"));
    _groupBox->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(_groupBox);

    QVBoxLayout * mainGroupLayout = new QVBoxLayout();

    _clipboardRadioButton = new QRadioButton(tr("Copy to clipboard"));
    _fileRadioButton = new QRadioButton(tr("File"));

    connect(_clipboardRadioButton, &QAbstractButton::toggled,
            this, &OutputTargetWidget::onModeRadioButtonToggled);
    connect(_fileRadioButton, &QAbstractButton::toggled,
            this, &OutputTargetWidget::onModeRadioButtonToggled);

    mainGroupLayout->addWidget(_clipboardRadioButton, 0, Qt::AlignTop);

    QGridLayout * gridLayout = new QGridLayout;

    gridLayout->addWidget(_fileRadioButton, 0, 0);

    QHBoxLayout * filenameLayout = new QHBoxLayout();

    _filenameEdit = new QLineEdit;
    _filenameSelectionButton = new QPushButton(
                QIcon(":/icons/folder_explore.png"),
                tr(""));
    /*connect(_filenameSelectionButton,
            &QAbstractButton::clicked,
            this,
            &OutputTargetWidget::onFilenameSelectionButtonClicked
    );*/
    _filenameSelectionButton->setMinimumWidth(30);

    filenameLayout->addWidget(_filenameEdit, 20);
    filenameLayout->addWidget(_filenameSelectionButton, 1);

    gridLayout->addLayout(filenameLayout, 0, 1);

    _encodingLabel = new QLabel(tr("Encoding:"));
    gridLayout->addWidget(_encodingLabel, 1, 0);

    _encodingCombobox = new QComboBox();
    // TODO: add search?
    connect(_encodingCombobox, &QComboBox::currentTextChanged,
            this, &OutputTargetWidget::onEncodingComboboxTextChanged);
    gridLayout->addWidget(_encodingCombobox, 1, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 4);
    gridLayout->setAlignment(Qt::AlignTop);

    mainGroupLayout->addLayout(gridLayout, 0);

    _groupBox->setLayout(mainGroupLayout);
}

void OutputTargetWidget::fillDataFromPresenter()
{
    if (_presenter->isModeClipboard()) {
        _clipboardRadioButton->blockSignals(true);
        _clipboardRadioButton->setChecked(true);
        _clipboardRadioButton->blockSignals(false);
    } else {
        _fileRadioButton->blockSignals(true);
        _fileRadioButton->setChecked(true);
        _fileRadioButton->blockSignals(false);
    }

    _encodingCombobox->blockSignals(true);
    _encodingCombobox->clear();
    _encodingCombobox->addItems(_presenter->supportedFileEncodings());
    _encodingCombobox->setCurrentText(_presenter->fileEncoding());
    _encodingCombobox->blockSignals(false);
}

void OutputTargetWidget::validateControls()
{
    _filenameEdit->setEnabled(_presenter->isModeFile());
    _filenameSelectionButton->setEnabled(_presenter->isModeFile());

    _encodingLabel->setEnabled(_presenter->isModeFile());
    _encodingCombobox->setEnabled(_presenter->isModeFile());
}

void OutputTargetWidget::onModeRadioButtonToggled(bool checked)
{
    if (checked) {
        if (sender() == _clipboardRadioButton) {
            _presenter->setModeClipboard();
        } else {
            _presenter->setModeFile();
        }
        validateControls();
    }
}

void OutputTargetWidget::onEncodingComboboxTextChanged(const QString & text)
{
    _presenter->setFileEncoding(text);
}

} // namespace export_query
} // namespace ui
} // namespace meow
