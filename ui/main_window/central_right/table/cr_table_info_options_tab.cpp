#include "cr_table_info_options_tab.h"
#include "models/forms/table_info_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

OptionsTab::OptionsTab(models::forms::TableInfoForm * form,
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

    // Auto increment
    _autoIncLabel = new QLabel(tr("Auto increment:"));
    _mainGridLayout->addWidget(_autoIncLabel, row, 0);

    _autoIncEdit = new QLineEdit();
    _autoIncEdit->setReadOnly(true);
    _autoIncEdit->setToolTip("Readonly"); // temp
    _autoIncLabel->setBuddy(_autoIncEdit);
    _mainGridLayout->addWidget(_autoIncEdit, row, 1);

    // Default collation
    _defaultCollationLabel = new QLabel(tr("Default collation:"));
    _mainGridLayout->addWidget(_defaultCollationLabel, row, 2);

    _defaultCollationComboBox = new QComboBox();
    _defaultCollationComboBox->setToolTip("Readonly");
    _defaultCollationLabel->setBuddy(_defaultCollationComboBox);
    _mainGridLayout->addWidget(_defaultCollationComboBox, row, 3);

    ++row; // ------------------------------------------------------------------

    // Average row length
    _avgRowLenLabel = new QLabel(tr("Average row length:"));
    _mainGridLayout->addWidget(_avgRowLenLabel, row, 0);

    _avgRowLenEdit = new QLineEdit();
    _avgRowLenEdit->setReadOnly(true);
    _avgRowLenEdit->setToolTip("Readonly"); // temp
    _avgRowLenLabel->setBuddy(_avgRowLenEdit);
    _mainGridLayout->addWidget(_avgRowLenEdit, row, 1);

    // Engine
    _engineLabel = new QLabel(tr("Engine:"));
    _mainGridLayout->addWidget(_engineLabel, row, 2);

    _engineComboBox = new QComboBox();
    _engineComboBox->setToolTip("Readonly");
    _engineLabel->setBuddy(_engineComboBox);
    _mainGridLayout->addWidget(_engineComboBox, row, 3);

    ++row; // ------------------------------------------------------------------

    // Maximum row count
    _maxRowCountLabel = new QLabel(tr("Maximum row count:"));
    _mainGridLayout->addWidget(_maxRowCountLabel, row, 0);

    _maxRowCountEdit = new QLineEdit();
    _maxRowCountEdit->setReadOnly(true);
    _maxRowCountEdit->setToolTip("Readonly"); // temp
    _maxRowCountLabel->setBuddy(_maxRowCountEdit);
    _mainGridLayout->addWidget(_maxRowCountEdit, row, 1);

    // Row format
    _rowFormatLabel = new QLabel(tr("Row format:"));
    _mainGridLayout->addWidget(_rowFormatLabel, row, 2);

    _rowFormatComboBox = new QComboBox();
    _rowFormatComboBox->setToolTip("Readonly");
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
    // readonly
    _checksumForRowsCheckBox->setAttribute(Qt::WA_TransparentForMouseEvents);
    _checksumForRowsCheckBox->setFocusPolicy(Qt::NoFocus);
    _checksumForRowsCheckBox->setToolTip("Readonly");
    // readonly
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
    _autoIncEdit->setText(_tableForm->autoInc());
    _avgRowLenEdit->setText(_tableForm->avgRowLen());
    _maxRowCountEdit->setText(_tableForm->maxRows());
    _checksumForRowsCheckBox->setChecked(_tableForm->isCheckSum());

    QStringList collations;
    collations << _tableForm->collation();
    _defaultCollationComboBox->clear();
    _defaultCollationComboBox->addItems(collations);

    QStringList engines;
    engines << _tableForm->engine();
    _engineComboBox->clear();
    _engineComboBox->addItems(engines);

    QStringList rowFormats;
    rowFormats << _tableForm->rowFormat();
    _rowFormatComboBox->clear();
    _rowFormatComboBox->addItems(rowFormats);
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
