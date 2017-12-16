#include "central_right_table_tab.h"
#include "cr_table_info.h"
#include "cr_table_columns.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableTab::TableTab(QWidget * parent) : QWidget(parent)
{
    createWidgets();
}

void TableTab::setTable(db::TableEntity * table)
{
    _tableInfo->setTable(table);
    _columns->setTable(table);
}

void TableTab::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _tableInfo = new TableInfo(this);
    _tableInfo->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _tableInfo->setMinimumSize(QSize(300, 220));

    _columns = new TableColumns(this);
    _columns->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _columns->setMinimumSize(QSize(300, 150));

    _mainSplitter->addWidget(_tableInfo);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_columns);
    _mainSplitter->setStretchFactor(1, 2);


}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
