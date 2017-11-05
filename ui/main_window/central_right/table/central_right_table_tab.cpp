#include "central_right_table_tab.h"
#include "cr_table_columns.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableTab::TableTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void TableTab::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _columns = new TableColumns(this);
    mainLayout->addWidget(_columns);
    _columns->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
