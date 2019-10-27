#include "top_widget.h"
#include "models/forms/export_database_form.h"

namespace meow {
namespace ui {
namespace export_database {

using Option = models::forms::MySQLDumpOption;

TopWidget::TopWidget(models::forms::ExportDatabaseForm *form, QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();

    _checkboxOptions = {
            { _databaseCreateCheckbox, Option::CreateDatabase },
            { _databaseDropCheckbox,   Option::AddDropDatabase },
            { _tablesCreateCheckbox,   Option::CreateTable },
            { _tablesDropCheckbox,     Option::AddDropTable },
            { _triggersCreateCheckbox, Option::Triggers },
            { _triggersDropCheckbox,   Option::AddDropTrigger },
            { _routinesCreateCheckbox, Option::Routines },
            { _eventsCreateCheckbox,   Option::Events }};

    connect(_form, &models::forms::ExportDatabaseForm::optionsChanged,
            this, &TopWidget::onFormOptionsChanged);
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


    _optionsDatabasesLabel = new QLabel(tr("Database(s):"));
    _mainGridLayout->addWidget(_optionsDatabasesLabel, row, 0);

    _databaseCreateCheckbox = new QCheckBox(tr("Create"));
    connect(_databaseCreateCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);
    _databaseDropCheckbox = new QCheckBox(tr("Drop"));
    connect(_databaseDropCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);

    QHBoxLayout * dbLayout = new QHBoxLayout();
    dbLayout->addWidget(_databaseCreateCheckbox);
    dbLayout->addWidget(_databaseDropCheckbox);
    dbLayout->addStretch(1);
    _mainGridLayout->addLayout(dbLayout, row, 1);

    row++;

    // -------------------------------------------------------------------------

    _optionsTablesLabel = new QLabel(tr("Table(s):"));
    _mainGridLayout->addWidget(_optionsTablesLabel, row, 0);

    _tablesCreateCheckbox = new QCheckBox(tr("Create"));
    connect(_tablesCreateCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);
    _tablesDropCheckbox = new QCheckBox(tr("Drop"));
    connect(_tablesDropCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);

    QHBoxLayout * tablesLayout = new QHBoxLayout();
    tablesLayout->addWidget(_tablesCreateCheckbox);
    tablesLayout->addWidget(_tablesDropCheckbox);
    tablesLayout->addStretch(1);
    _mainGridLayout->addLayout(tablesLayout, row, 1);

    row++;

    // -------------------------------------------------------------------------

    _optionsTriggersLabel = new QLabel(tr("Trigger(s):"));
    _mainGridLayout->addWidget(_optionsTriggersLabel, row, 0);

    _triggersCreateCheckbox = new QCheckBox(tr("Create"));
    connect(_triggersCreateCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);
    _triggersDropCheckbox = new QCheckBox(tr("Drop"));
    connect(_triggersDropCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);

    QHBoxLayout * triggersLayout = new QHBoxLayout();
    triggersLayout->addWidget(_triggersCreateCheckbox);
    triggersLayout->addWidget(_triggersDropCheckbox);
    triggersLayout->addStretch(1);
    _mainGridLayout->addLayout(triggersLayout, row, 1);

    row++;

    // -------------------------------------------------------------------------

    _optionsRoutinesEventsLabel = new QLabel(tr("Dump:"));
    _mainGridLayout->addWidget(_optionsRoutinesEventsLabel, row, 0);

    _routinesCreateCheckbox = new QCheckBox(tr("Routines"));
    connect(_routinesCreateCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);
    _eventsCreateCheckbox = new QCheckBox(tr("Events"));
    connect(_eventsCreateCheckbox, &QCheckBox::stateChanged,
            this, &TopWidget::onOptionsCheckboxChanged);

    QHBoxLayout * routinesEventsLayout = new QHBoxLayout();
    routinesEventsLayout->addWidget(_routinesCreateCheckbox);
    routinesEventsLayout->addWidget(_eventsCreateCheckbox);
    routinesEventsLayout->addStretch(1);
    _mainGridLayout->addLayout(routinesEventsLayout, row, 1);

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

    clearResults();
    appendToResults(_form->currentCommand());

    _filenameEdit->blockSignals(true);
    _filenameEdit->setText(_form->filename());
    _filenameEdit->blockSignals(false);

    _databaseToExportComboBox->blockSignals(true);
    _databaseToExportComboBox->clear();
    _databaseToExportComboBox->addItems(_form->databases());
    if (_form->allDatabases()) {
        _databaseToExportComboBox->setCurrentIndex(0);
    } else {
        _databaseToExportComboBox->setCurrentIndex(
            _databaseToExportComboBox->findText(_form->database()));
    }
    _databaseToExportComboBox->blockSignals(false);


    auto it = _checkboxOptions.constBegin();
    while (it != _checkboxOptions.constEnd()) {

        QCheckBox * checkbox = it.key();
        Option option = it.value();

        checkbox->blockSignals(true);
        checkbox->setCheckState(
            _form->isOptionEnabled(option) ?
            Qt::CheckState::Checked : Qt::CheckState::Unchecked);
        checkbox->blockSignals(false);

        ++it;
    }

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

void TopWidget::onOptionsCheckboxChanged(int state)
{
    // Listening: Kampfar - Tornekratt

    const auto it = _checkboxOptions.find(static_cast<QCheckBox *>(sender()));

    if (it != _checkboxOptions.end()) {
        _form->setOption(it.value(),
                         state == Qt::Checked);
    }
}

void TopWidget::onFormOptionsChanged()
{
    // TODO: don't change all fields
    fillDataFromForm();
}

} // namespace export_database
} // namespace ui
} // namespace meow

