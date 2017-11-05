#include "cr_table_columns_tools.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableColumnsTools::TableColumnsTools(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void TableColumnsTools::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(2, 2, 0, 0);
    setLayout(mainLayout);

    _columnsLabel = new QLabel(tr("Columns:"));
    mainLayout->addWidget(_columnsLabel);

    // TODO: add buttons
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
