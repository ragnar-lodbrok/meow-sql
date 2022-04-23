#include "cr_data_filter_widget.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

DataFilterWidget::DataFilterWidget(models::DataTableModel * dataTableModel,
                                   QWidget * parent)
    : QWidget(parent)
    , _form(dataTableModel)
{
    // Listening: Zeal & Ardor - Götterdämmerung
    createWidgets();
    validateControls();

    connect(&_form,
            &ui::presenters::CentralRightDataFilterForm::SQLFilterTextGenerated,
            this, &DataFilterWidget::onSQLFilterTextGenerated);

    connect(&_form,
            &ui::presenters::CentralRightDataFilterForm::recentFiltersChanged,
            this, &DataFilterWidget::onRecentFiltersChanged);
}

void DataFilterWidget::setDBEntity(db::Entity * tableOrViewEntity)
{
    _editFilterSearch->clear();
    _sqlEditor->clear();
    _form.setDBEntity(tableOrViewEntity);
    fillRecentFilters(); // TODO: lazy load
    validateControls();
}

void DataFilterWidget::createWidgets()
{
    // Listening: Ex Deo - I caligvla
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout * leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop);

    _recentFiltersWidget = new QWidget();
    QHBoxLayout * recentLayout = new QHBoxLayout();
    _labelRecentFilters = new QLabel(tr("Recent filters:"));
    recentLayout->addWidget(_labelRecentFilters);
    _comboboxRecentFilters = new QComboBox();
    connect(_comboboxRecentFilters,
#if QT_VERSION >= QT_VERSION_CHECK(5, 7, 0)
            QOverload<int>::of(&QComboBox::activated),
#else
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
#endif
            this,
            &DataFilterWidget::onComboboxRecentFiltersActivated);
    recentLayout->addWidget(_comboboxRecentFilters, 1);
    _recentFiltersWidget->setLayout(recentLayout);
    leftLayout->addWidget(_recentFiltersWidget);

    _sqlEditor = new ui::common::SQLEditor();
    _sqlEditor->setWordWrapMode(QTextOption::WordWrap);
    _sqlEditor->setHeightByRowCount(3);
    connect(_sqlEditor, &QPlainTextEdit::textChanged,
            this, &DataFilterWidget::onSQLFilterTextChanged);
    leftLayout->addWidget(_sqlEditor);


    QVBoxLayout * rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop);

    _labelColumnFilter = new QLabel(tr("Create multi column filter:"));
    rightLayout->addWidget(_labelColumnFilter);

    _editFilterSearch = new QLineEdit();
    connect(_editFilterSearch, &QLineEdit::textChanged,
            this, &DataFilterWidget::onFilterEditChanged);
    rightLayout->addWidget(_editFilterSearch);

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    _applyFilterButton = new QPushButton(tr("Apply filter"));
    connect(_applyFilterButton, &QAbstractButton::clicked,
            this, &DataFilterWidget::onApplyFilterButtonClicked);
    _clearFilterButton = new QPushButton(tr("Clear"));
    connect(_clearFilterButton, &QAbstractButton::clicked,
            this, &DataFilterWidget::onClearFilterButtonClicked);
    buttonsLayout->addWidget(_applyFilterButton);
    buttonsLayout->addWidget(_clearFilterButton);
    rightLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(leftLayout, 5);
    mainLayout->addLayout(rightLayout, 1);

    setLayout(mainLayout);
}

void DataFilterWidget::validateControls()
{
    _recentFiltersWidget->setVisible(_form.hasRecentFilters());
    _clearFilterButton->setEnabled(!_sqlEditor->toPlainText().isEmpty());
}

void DataFilterWidget::fillRecentFilters()
{
    _comboboxRecentFilters->clear();
    _comboboxRecentFilters->insertItems(0, _form.recentFilters());
    _comboboxRecentFilters->setCurrentIndex(
        _comboboxRecentFilters->count() - 1); // select latest
}

void DataFilterWidget::onFilterEditChanged(const QString &text)
{
    _form.setFilterEditText(text);
}

void DataFilterWidget::onSQLFilterTextGenerated(const QString &text)
{
    _sqlEditor->setPlainText(text);
    _clearFilterButton->setEnabled(!text.isEmpty());
}

void DataFilterWidget::onSQLFilterTextChanged()
{
    _clearFilterButton->setEnabled(!_sqlEditor->toPlainText().isEmpty());
}

void DataFilterWidget::onApplyFilterButtonClicked()
{
    try {
        _form.applyWhereFilter(_sqlEditor->toPlainText());
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void DataFilterWidget::onClearFilterButtonClicked()
{
    _editFilterSearch->clear();
    _sqlEditor->clear();
    onApplyFilterButtonClicked();
}

void DataFilterWidget::onRecentFiltersChanged()
{
    fillRecentFilters();
    validateControls();
}

void DataFilterWidget::onComboboxRecentFiltersActivated(int index)
{
    QString recentText = _comboboxRecentFilters->itemText(index);
    if (!recentText.isEmpty()) {
        _sqlEditor->setPlainText(recentText);
    }
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

