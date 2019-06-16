#include "top_widget.h"
#include "models/forms/export_database_form.h"

namespace meow {
namespace ui {
namespace export_database {

TopWidget::TopWidget(models::forms::ExportDatabaseForm *form, QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}

void TopWidget::createWidgets()
{
    _mainGridLayout = new QGridLayout();
    int row = 0;

    // -------------------------------------------------------------------------

    _databaseToExportLabel = new QLabel(tr("Database:"));
    _mainGridLayout->addWidget(_databaseToExportLabel, row, 0);
    _databaseToExportComboBox = new QComboBox();
    _databaseToExportLabel->setBuddy(_databaseToExportComboBox);
    _mainGridLayout->addWidget(_databaseToExportComboBox, row, 1);

    connect(_databaseToExportComboBox,
            &QComboBox::currentTextChanged,
            [=](const QString & text) {

                bool isAllDatabases
                    = _databaseToExportComboBox->currentIndex() == 0;

                _form->setAllDatabases(isAllDatabases);

                if (isAllDatabases == false) {
                    _form->setDatabase(text);
                }
            });

    row++;

    // -------------------------------------------------------------------------


    _filenameLabel = new QLabel(tr("Filename:"));
    _mainGridLayout->addWidget(_filenameLabel, row, 0);
    _filenameEdit = new QLineEdit;
    _filenameLabel->setBuddy(_filenameEdit);
    _mainGridLayout->addWidget(_filenameEdit, row, 1);
    connect(_filenameEdit, &QLineEdit::textEdited,
            [=](const QString &newName) {
                _form->setFilename(newName);
            });

    row++;

    // -------------------------------------------------------------------------

    _results = new QPlainTextEdit;
    _results->setReadOnly(true);
    _mainGridLayout->addWidget(_results, row, 0, row, 2);

    row++;

    // -------------------------------------------------------------------------

    _mainGridLayout->setColumnMinimumWidth(0, 150);
    _mainGridLayout->setColumnStretch(1, 2);
    this->setLayout(_mainGridLayout);
}

void TopWidget::fillDataFromForm()
{
    _filenameEdit->blockSignals(true);
    _filenameEdit->setText(_form->filename());
    _filenameEdit->blockSignals(false);

    _databaseToExportComboBox->blockSignals(true);
    _databaseToExportComboBox->clear();
    _databaseToExportComboBox->addItems(_form->databases());
    _databaseToExportComboBox->setCurrentIndex(
        _databaseToExportComboBox->findText(_form->database()));
    _databaseToExportComboBox->blockSignals(false);
}

void TopWidget::clearResults()
{
    _results->clear();
}

void TopWidget::appendToResults(const QString & str)
{
    _results->insertPlainText(str);
    _results->moveCursor(QTextCursor::End);
}


} // namespace export_database
} // namespace ui
} // namespace meow

