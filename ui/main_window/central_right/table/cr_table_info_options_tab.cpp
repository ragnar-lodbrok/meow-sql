#include "cr_table_info_options_tab.h"
#include "ui/presenters/table_info_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

OptionsTab::OptionsTab(presenters::TableInfoForm * form,
                       QWidget *parent)
    : QWidget(parent),
      _tableForm(form)
{
    Q_CHECK_PTR(form);
    createWidgets();
}

void OptionsTab::refreshData()
{
    fillDataFromForm();
}

void OptionsTab::createWidgets()
{
    _mainGridLayout = new QGridLayout();
    int row = 0;

    auto positiveIntValidator = new QIntValidator(this);
    positiveIntValidator->setBottom(0);

    // Auto increment
    _autoIncLabel = new QLabel(tr("Auto increment:"));
    _mainGridLayout->addWidget(_autoIncLabel, row, 0);

    _autoIncEdit = new QLineEdit();
    _autoIncLabel->setBuddy(_autoIncEdit);
    _autoIncEdit->setValidator(positiveIntValidator);
    connect(_autoIncEdit, &QLineEdit::textChanged,
            [=](const QString &value) {
                _tableForm->setAutoInc(value);
            });
    _mainGridLayout->addWidget(_autoIncEdit, row, 1);

    // Default collation
    _defaultCollationLabel = new QLabel(tr("Default collation:"));
    _mainGridLayout->addWidget(_defaultCollationLabel, row, 2);

    _defaultCollationComboBox = new QComboBox();
    _defaultCollationLabel->setBuddy(_defaultCollationComboBox);
    connect(_defaultCollationComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                _tableForm->setCollation(text);
            });
    _mainGridLayout->addWidget(_defaultCollationComboBox, row, 3);

    ++row; // ------------------------------------------------------------------

    // Average row length
    _avgRowLenLabel = new QLabel(tr("Average row length:"));
    _mainGridLayout->addWidget(_avgRowLenLabel, row, 0);

    _avgRowLenEdit = new QLineEdit();
    _avgRowLenEdit->setValidator(positiveIntValidator);
    connect(_avgRowLenEdit, &QLineEdit::textChanged,
            [=](const QString &value) {
                _tableForm->setAvgRowLen(value);
            });
    _avgRowLenLabel->setBuddy(_avgRowLenEdit);
    _mainGridLayout->addWidget(_avgRowLenEdit, row, 1);

    // Engine
    _engineLabel = new QLabel(tr("Engine:"));
    _mainGridLayout->addWidget(_engineLabel, row, 2);

    _engineComboBox = new QComboBox();
    connect(_engineComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                _tableForm->setEngine(text);
            });
    _engineLabel->setBuddy(_engineComboBox);
    _mainGridLayout->addWidget(_engineComboBox, row, 3);

    ++row; // ------------------------------------------------------------------

    // Maximum row count
    _maxRowCountLabel = new QLabel(tr("Maximum row count:"));
    _mainGridLayout->addWidget(_maxRowCountLabel, row, 0);

    _maxRowCountEdit = new QLineEdit();
    _maxRowCountEdit->setValidator(positiveIntValidator);
    connect(_maxRowCountEdit, &QLineEdit::textChanged,
            [=](const QString &value) {
                _tableForm->setMaxRows(value);
            });
    _maxRowCountLabel->setBuddy(_maxRowCountEdit);
    _mainGridLayout->addWidget(_maxRowCountEdit, row, 1);

    // Row format
    _rowFormatLabel = new QLabel(tr("Row format:"));
    _mainGridLayout->addWidget(_rowFormatLabel, row, 2);

    _rowFormatComboBox = new QComboBox();
    connect(_rowFormatComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {
                _tableForm->setRowFormat(text);
            });
    _rowFormatLabel->setBuddy(_rowFormatComboBox);
    _mainGridLayout->addWidget(_rowFormatComboBox, row, 3);

    // Union tables
    //_unionTablesLabel = new QLabel(tr("Union tables:"));
    //_mainGridLayout->addWidget(_unionTablesLabel, row, 2);

    //_unionTablesEdit = new QPlainTextEdit();
    //_unionTablesEdit->setReadOnly(true);
    //_unionTablesEdit->setToolTip("Readonly"); // temp
    //_unionTablesLabel->setBuddy(_unionTablesEdit);
    //_mainGridLayout->addWidget(_unionTablesEdit, row, 3 , 2, 1);

    ++row; // ------------------------------------------------------------------

    // Checksum for rows
    _checksumForRowsCheckBox = new QCheckBox(tr("Checksum for rows")); // ":" removed from the label
    connect(_checksumForRowsCheckBox, &QCheckBox::stateChanged,
            [=](int state) {
                _tableForm->setCheckSum(state == Qt::Checked);
            });
    _mainGridLayout->addWidget(_checksumForRowsCheckBox, row, 0, 1, 2);

    //++row; // ------------------------------------------------------------------

    // INSERT method
    //_insertMethodLabel = new QLabel(tr("INSERT method:"));
    //_mainGridLayout->addWidget(_insertMethodLabel, row, 2);

    //_insertMethodComboBox = new QComboBox();
    //_insertMethodComboBox->setToolTip("Readonly");
    //_insertMethodLabel->setBuddy(_insertMethodComboBox);
    //_mainGridLayout->addWidget(_insertMethodComboBox, row, 3);

    _mainGridLayout->setColumnStretch(0, 0);
    _mainGridLayout->setColumnStretch(1, 1);
    _mainGridLayout->setColumnStretch(2, 0);
    _mainGridLayout->setColumnStretch(3, 2);
    _mainGridLayout->setAlignment(Qt::AlignTop);
    this->setLayout(_mainGridLayout);
}

void OptionsTab::fillDataFromForm()
{
    _autoIncEdit->blockSignals(true);
    _autoIncEdit->setText(_tableForm->autoInc());
    _autoIncEdit->blockSignals(false);
    _autoIncEdit->setEnabled(_tableForm->isEditingSupported());

    _avgRowLenEdit->blockSignals(true);
    _avgRowLenEdit->setText(_tableForm->avgRowLen());
    _avgRowLenEdit->blockSignals(false);
    _avgRowLenEdit->setEnabled(_tableForm->isEditingSupported());

    _maxRowCountEdit->blockSignals(true);
    _maxRowCountEdit->setText(_tableForm->maxRows());
    _maxRowCountEdit->blockSignals(false);
    _maxRowCountEdit->setEnabled(_tableForm->isEditingSupported());

    _checksumForRowsCheckBox->blockSignals(true);
    _checksumForRowsCheckBox->setChecked(_tableForm->isCheckSum());
    _checksumForRowsCheckBox->blockSignals(false);
    _checksumForRowsCheckBox->setEnabled(_tableForm->isEditingSupported());

    _defaultCollationComboBox->blockSignals(true);
    _defaultCollationComboBox->clear(); // TODO: lazy load?
    _defaultCollationComboBox->addItems(_tableForm->allCollations());
    _defaultCollationComboBox->setCurrentIndex(
        _defaultCollationComboBox->findText(_tableForm->collation()));
    _defaultCollationComboBox->blockSignals(false);
    _defaultCollationComboBox->setEnabled(_tableForm->isEditingSupported());

    _engineComboBox->blockSignals(true);
    _engineComboBox->clear();
    _engineComboBox->addItems(_tableForm->allEngines()); // TODO: default
    _engineComboBox->setCurrentIndex(
        _engineComboBox->findText(_tableForm->engine()));
    _engineComboBox->blockSignals(false);
    _engineComboBox->setEnabled(_tableForm->isEditingSupported());

    _rowFormatComboBox->blockSignals(true);
    _rowFormatComboBox->clear();
    _rowFormatComboBox->addItems(_tableForm->allRowFormats());
    _rowFormatComboBox->setCurrentIndex(
        _rowFormatComboBox->findText(_tableForm->rowFormat()));
    _rowFormatComboBox->blockSignals(false);
    _rowFormatComboBox->setEnabled(_tableForm->isEditingSupported());

}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
