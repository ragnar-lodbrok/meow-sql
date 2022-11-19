#include "cr_query_data_tab.h"
#include "app/app.h"
#include "ui/export_query/export_query_data_dialog.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryDataTab::QueryDataTab(db::QueryDataPtr queryData, QWidget *parent)
    : QWidget(parent),
      _model(queryData)
{
    _model.setRowCount(-1); // Temp: take row/col count from query data
    _model.setColumnCount(-1);

    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2, 2, 2, 2);
    setLayout(mainLayout);

    _dataTable = new EditableQueryDataTableView(&_model);
    _dataTable->verticalHeader()->hide();
    _dataTable->horizontalHeader()->setHighlightSections(false);
    auto geometrySettings = meow::app()->settings()->geometrySettings();
    _dataTable->verticalHeader()->setDefaultSectionSize(
       geometrySettings->tableViewDefaultRowHeight());
    _dataTable->horizontalHeader()->setResizeContentsPrecision(
        meow::app()->settings()->textSettings()->tableAutoResizeRowsLookupCount()
    );
    connect(_dataTable->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &QueryDataTab::onDataTableHeaderClicked);
    _dataTable->setSelectionBehavior(
        QAbstractItemView::SelectionBehavior::SelectRows);

    _dataTable->setModel(_model.createSortFilterModel());
    mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false);

    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }
}

QueryDataTab::~QueryDataTab()
{

}

void QueryDataTab::setFilterPattern(const QString & pattern, bool regexp)
{
    _model.setFilterPattern(pattern, regexp);
}

int QueryDataTab::filterMatchedRowCount() const
{
    return _model.filterMatchedRowCount();
}

int QueryDataTab::totalRowCount() const
{
    return _model.rowCount();
}

void QueryDataTab::onDataExportAction()
{
    meow::ui::export_query::Dialog dialog;
    dialog.setData(&_model, _dataTable->selectionModel(), _dataTable);
    dialog.exec();
}

void QueryDataTab::onDataTableHeaderClicked(int column)
{
    QHeaderView * header = _dataTable->horizontalHeader();

    QSortFilterProxyModel * model
            = static_cast<QSortFilterProxyModel *>(_dataTable->model());
    int curentSortColumn = model->sortColumn();

    if (curentSortColumn == static_cast<int>(-1)
            || (curentSortColumn != column)) {

        header->setSortIndicator(column, Qt::AscendingOrder);
        header->setSortIndicatorShown(true);
        model->sort(column, Qt::AscendingOrder);

    } else {

        Qt::SortOrder currentSortOrder = model->sortOrder();

        if (currentSortOrder == Qt::AscendingOrder) {
            header->setSortIndicator(column, Qt::DescendingOrder);
            header->setSortIndicatorShown(true);
            model->sort(column, Qt::DescendingOrder);
        } else {
            header->setSortIndicatorShown(false);
            model->sort(-1); // no sort
        }
    }

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
