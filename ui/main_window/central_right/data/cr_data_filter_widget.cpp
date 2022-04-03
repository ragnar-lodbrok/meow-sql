#include "cr_data_filter_widget.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

DataFilterWidget::DataFilterWidget(QWidget *parent) : QWidget(parent)
{
    // Listening: Zeal & Ardor - Götterdämmerung
    createWidgets();
}

void DataFilterWidget::setDBEntity(db::Entity * tableOrViewEntity)
{
    _form.setDBEntity(tableOrViewEntity);
}

void DataFilterWidget::createWidgets()
{
    // Listening: Ex Deo - I caligvla
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout * leftLayout = new QVBoxLayout();
    leftLayout->setAlignment(Qt::AlignTop);

    QHBoxLayout * recentLayout = new QHBoxLayout();
    _labelRecentFilters = new QLabel(tr("Recent filters:"));
    recentLayout->addWidget(_labelRecentFilters);
    _comboboxRecentFilters = new QComboBox();
    recentLayout->addWidget(_comboboxRecentFilters, 1);
    leftLayout->addLayout(recentLayout);

    _sqlEditor = new ui::common::SQLEditor();
    _sqlEditor->setWordWrapMode(QTextOption::WordWrap);
    _sqlEditor->setHeightByRowCount(3);
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
    _clearFilterButton = new QPushButton(tr("Clear"));
    buttonsLayout->addWidget(_applyFilterButton);
    buttonsLayout->addWidget(_clearFilterButton);
    rightLayout->addLayout(buttonsLayout);

    mainLayout->addLayout(leftLayout, 5);
    mainLayout->addLayout(rightLayout, 1);

    setLayout(mainLayout);
}

void DataFilterWidget::onFilterEditChanged(const QString &text)
{
    _form.setFilterEditText(text);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

