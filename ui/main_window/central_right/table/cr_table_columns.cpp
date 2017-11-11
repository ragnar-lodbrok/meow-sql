#include "cr_table_columns.h"
#include "central_right_table_tab.h"
#include "cr_table_columns_tools.h"

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
}

void TableColumns::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(mainLayout);

    _tools = new TableColumnsTools(this);
    mainLayout->addWidget(_tools);

    _columnsTable = new QTableView(this);
    _columnsTable->setModel(&_model);
    mainLayout->addWidget(_columnsTable);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
