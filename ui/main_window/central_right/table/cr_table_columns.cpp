#include "cr_table_columns.h"
#include "central_right_table_tab.h"
#include "cr_table_columns_tools.h"
#include "app/app.h"
#include "ui/models/table_columns_model.h"
#include "ui/delegates/table_column_type_delegate.h"
#include "ui/delegates/table_column_default_delegate.h"
#include "ui/delegates/table_column_collation_delegate.h"
#include "ui/delegates/checkbox_delegate.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TableColumnsVerticalHeaderView : public QHeaderView
{
public:

explicit TableColumnsVerticalHeaderView(Qt::Orientation orientation,
                                        QWidget *parent = nullptr)

        : QHeaderView(orientation, parent)
{

}


virtual void paintSection(QPainter *painter,
                          const QRect &rect,
                          int logicalIndex) const override
{

   QHeaderView::paintSection(painter, rect, logicalIndex);


   // Trick paint icon over parent's painting
   QPainter newPainter(painter->device());
   newPainter.translate(rect.topLeft());

   auto tableColumnsModel
            = static_cast<meow::ui::models::TableColumnsModel *>(model());

   QPixmap icon
           = tableColumnsModel->headerRowIcon(logicalIndex).value<QPixmap>();

   if (!icon.isNull()) {
       int y = (rect.height() - icon.size().height()) / 2;
       newPainter.drawPixmap(0, y, icon);
   }
}

};

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
        (int)models::TableColumnsModel::Columns::Name);
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
    _columnsTable->setVerticalHeader(
                new TableColumnsVerticalHeaderView(Qt::Vertical, _columnsTable));
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

    delegates::TableColumnTypeDelegate * typeDelegate
        = new delegates::TableColumnTypeDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::DataType,
        typeDelegate);

    delegates::TableColumnDefaultDelegate * defaultDelegate
        = new delegates::TableColumnDefaultDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::Default,
        defaultDelegate);

    delegates::TableColumnCollationDelegate * collationDelegate
        = new delegates::TableColumnCollationDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::Collation,
        collationDelegate);

    delegates::CheckboxDelegate * checkboxDelegate
        = new delegates::CheckboxDelegate(&_model);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::Unsigned,
        checkboxDelegate);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::AllowNull,
        checkboxDelegate);
    _columnsTable->setItemDelegateForColumn(
        (int)models::TableColumnsModel::Columns::Zerofill,
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
