#include "central_right_query_tab.h"
#include "cr_query_panel.h"
#include "cr_query_result.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryTab::QueryTab(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

void QueryTab::createWidgets()
{
    _mainLayout = new QHBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _mainVerticalSplitter = new QSplitter(Qt::Vertical);
    _mainVerticalSplitter->setChildrenCollapsible(false);
    _mainLayout->addWidget(_mainVerticalSplitter);

    _queryPanel = new QueryPanel();
    _queryPanel->setMinimumHeight(80);
    _mainVerticalSplitter->addWidget(_queryPanel);
    _mainVerticalSplitter->setStretchFactor(0, 1);

    _queryResult = new QueryResult();
    _queryResult->setMinimumHeight(40);
    _mainVerticalSplitter->addWidget(_queryResult);
    _mainVerticalSplitter->setStretchFactor(1, 2);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
