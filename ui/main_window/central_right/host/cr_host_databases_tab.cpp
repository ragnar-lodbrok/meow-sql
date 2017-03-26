#include "cr_host_databases_tab.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

HostDatabasesTab::HostDatabasesTab(QWidget *parent) : QWidget(parent)
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    createDatabasesTable();
}

void HostDatabasesTab::createDatabasesTable()
{

    _databasesTable = new QTableView();
    _databasesTable->verticalHeader()->hide();
    _databasesTable->horizontalHeader()->setHighlightSections(false);

    //_databasesTable->setModel();
    _mainLayout->addWidget(_databasesTable);
    _databasesTable->setSortingEnabled(false); // TODO
    _databasesTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    _databasesTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

