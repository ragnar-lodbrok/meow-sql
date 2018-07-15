#include "cr_query_data_tab.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryDataTab::QueryDataTab(db::QueryData * queryData, QWidget *parent)
    : QWidget(parent),
      _model(queryData)
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2, 2, 2, 2);
    setLayout(mainLayout);

    _dataTable = new TableView();
    _dataTable->verticalHeader()->hide();
    _dataTable->horizontalHeader()->setHighlightSections(false);
    auto geometrySettings = meow::app()->settings()->geometrySettings();
    _dataTable->verticalHeader()->setDefaultSectionSize(
       geometrySettings->tableViewDefaultRowHeight());
    _dataTable->horizontalHeader()->setResizeContentsPrecision(
        meow::app()->settings()->textSettings()->tableAutoResizeRowsLookupCount()
    );

    _dataTable->setModel(&_model);
    mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false); // TODO

    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }
}

QueryDataTab::~QueryDataTab()
{

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
