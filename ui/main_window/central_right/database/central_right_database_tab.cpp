#include "central_right_database_tab.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

DatabaseTab::DatabaseTab(QWidget *parent)
    : BaseRootTab(BaseRootTab::Type::DataBase, parent)
    , _model()
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(2, 2, 2, 2);
    this->setLayout(_mainLayout);

    createEntitiesTable();
}

void DatabaseTab::createEntitiesTable()
{

    _entitiesTable = new QTableView();
    _entitiesTable->verticalHeader()->hide();
    _entitiesTable->horizontalHeader()->setHighlightSections(false);
    auto geometrySettings = meow::app()->settings()->geometrySettings();
    _entitiesTable->verticalHeader()->setDefaultSectionSize(
       geometrySettings->tableViewDefaultRowHeight());

    //_entitiesTable->setModel(&_model);
    _proxyEntitiesModel.setSourceModel(&_model);
    _entitiesTable->setModel(&_proxyEntitiesModel);
    _entitiesTable->setSortingEnabled(true);
    _entitiesTable->sortByColumn(
        static_cast<int>(models::db::DatabaseEntitiesTableModel::Columns::Name),
        Qt::AscendingOrder);
    // TODO: natural sorting for numbers
    // TODO: filter by Filter panel

    _mainLayout->addWidget(_entitiesTable);
    _entitiesTable->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);
    _entitiesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int i=0; i<_model.columnCount(); ++i) {
        _entitiesTable->setColumnWidth(i, _model.columnWidth(i));
    }
}

void DatabaseTab::setDataBase(meow::db::DataBaseEntity * database)
{
    _model.setDataBase(database);
}


} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
