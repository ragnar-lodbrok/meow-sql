#include "cr_table_columns.h"
#include "central_right_table_tab.h"
#include "cr_table_columns_tools.h"
#include "app/app.h"
#include "models/db/table_columns_model.h"
#include "models/delegates/table_column_type_delegate.h"
#include "models/delegates/table_column_default_delegate.h"
#include "models/delegates/table_column_collation_delegate.h"
#include "models/delegates/checkbox_delegate.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableColumns::TableColumns(TableTab * parent) : QWidget(parent)
{
    createWidgets();
}

void TableColumns::setTable(db::TableEntity * table)
{
    _model.setTable(table);
    validateControls();
}

void TableColumns::actionAddColumn(bool checked)
{
    Q_UNUSED(checked);

    // insert
    QModelIndex curIndex = _columnsTable->selectionModel()->currentIndex();
    int curIntIndex = curIndex.isValid() ? curIndex.row() : -1;
    int newRowIntIndex = _model.insertEmptyDefaultRow(curIntIndex);

    // select
    QModelIndex newRowNameIndex = _columnsTable->model()->index(
        newRowIntIndex,
        (int)meow::models::db::TableColumnsModel::Columns::Name);
    _columnsTable->selectionModel()
        ->setCurrentIndex(newRowNameIndex,
                          QItemSelectionModel::Clear);

    // scroll
    _columnsTable->scrollTo(_columnsTable->selectionModel()->currentIndex());

     // edit name
    _columnsTable->edit(newRowNameIndex);
}

void TableColumns::actionRemoveColumn(bool checked)
{
    Q_UNUSED(checked);
    QModelIndex curIndex = _columnsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        _model.removeRowAt(curIndex.row());
        validateControls();
    }
}

void TableColumns::actionMoveColumnUp(bool checked)
{
    Q_UNUSED(checked);
    QModelIndex curIndex = _columnsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        if (_model.moveRowUp(curIndex.row())) {
            scrollToCurrent();
            validateControls();
        }
    }
}

void TableColumns::actionMoveColumnDown(bool checked)
{
    Q_UNUSED(checked);
    QModelIndex curIndex = _columnsTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        if (_model.moveRowDown(curIndex.row())) {
            scrollToCurrent();
            validateControls();
        }
    }
}

void TableColumns::validateControls()
{
    using ColAction = TableColumnsTools::Action;

    QModelIndex curIndex = _columnsTable->selectionModel()->currentIndex();

    if (curIndex.isValid() && _model.hasTable()) {
        int row = curIndex.row();
        _tools->setActionEnabled(ColAction::Add,      _model.canAddRow());
        _tools->setActionEnabled(ColAction::Remove,   _model.canRemoveRow(row));
        _tools->setActionEnabled(ColAction::MoveUp,   _model.canMoveRowUp(row));
        _tools->setActionEnabled(ColAction::MoveDown,
                                 _model.canMoveRowDown(row));
    } else {
        _tools->setActionEnabled(ColAction::Add,      _model.canAddRow());
        _tools->setActionEnabled(ColAction::Remove,   false);
        _tools->setActionEnabled(ColAction::MoveUp,   false);
        _tools->setActionEnabled(ColAction::MoveDown, false);
    }
}

void TableColumns::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    _tools = new TableColumnsTools(this);
    mainLayout->addWidget(_tools);

    _columnsTable = new QTableView(this);
    _columnsTable->horizontalHeader()->setHighlightSections(false);
    _columnsTable->verticalHeader()->setHighlightSections(false);
    auto geometrySettings = meow::app()->settings()->geometrySettings();
    _columnsTable->verticalHeader()->setDefaultSectionSize(
       geometrySettings->tableViewDefaultRowHeight());
    _columnsTable->setModel(&_model);
    for (int i=0; i<_model.columnCount(); ++i) {
        _columnsTable->setColumnWidth(i, _model.columnWidth(i));
    }
    mainLayout->addWidget(_columnsTable);
    _columnsTable->setSortingEnabled(false);
    _columnsTable->setSelectionBehavior(
          QAbstractItemView::SelectionBehavior::SelectRows);
    _columnsTable->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(_columnsTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &TableColumns::currentRowChanged
    );

    models::delegates::TableColumnTypeDelegate * typeDelegate
        = new models::delegates::TableColumnTypeDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::DataType,
        typeDelegate);

    models::delegates::TableColumnDefaultDelegate * defaultDelegate
        = new models::delegates::TableColumnDefaultDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::Default,
        defaultDelegate);

    models::delegates::TableColumnCollationDelegate * collationDelegate
        = new models::delegates::TableColumnCollationDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::Collation,
        collationDelegate);

    models::delegates::CheckboxDelegate * checkboxDelegate
        = new models::delegates::CheckboxDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::Unsigned,
        checkboxDelegate);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::AllowNull,
        checkboxDelegate);
    _columnsTable->setItemDelegateForColumn(
        (int)models::db::TableColumnsModel::Columns::Zerofill,
        checkboxDelegate);

}

void TableColumns::currentRowChanged(const QModelIndex &current,
                                     const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);
    validateControls();
}

void TableColumns::scrollToCurrent()
{
    _columnsTable->scrollTo(_columnsTable->selectionModel()->currentIndex());
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
