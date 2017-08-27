#include "cr_query_data_tab.h"

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

    _dataTable = new QTableView();
    _dataTable->verticalHeader()->hide();
    _dataTable->horizontalHeader()->setHighlightSections(false);

    _dataTable->setModel(&_model);
    mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false); // TODO
}

QueryDataTab::~QueryDataTab()
{

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
