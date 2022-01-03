#include "cr_query_result.h"
#include "cr_query_data_tab.h"
#include "ui/presenters/central_right_query_presenter.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryResult::QueryResult(presenters::CentralRightQueryPresenter * presenter,
                         QWidget *parent)
    : QWidget(parent),
      _presenter(presenter)
{

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    _dataTabs = new QTabWidget();
    layout->addWidget(_dataTabs);

    _dataTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

#ifdef Q_OS_MACOS
    _dataTabs->setDocumentMode(true);
#endif

}

QueryResult::~QueryResult()
{
    removeAllDataTabs();
}

void QueryResult::showAllQueriesData()
{
    removeAllDataTabs();
    int resultsCount = _presenter->resultsDataCount();
    for (int i=0; i<resultsCount; ++i) {
        QueryDataTab * dataTab = new QueryDataTab(_presenter->resultsDataAt(i));
        _dataTabs->addTab(dataTab, _presenter->resultTabCaption(i));
    }
}

void QueryResult::hideAllQueriesData()
{
    removeAllDataTabs();
}

void QueryResult::showQueryData(int queryIndex)
{
    db::QueryDataPtr queryData = _presenter->resultsDataAt(queryIndex);
    Q_ASSERT(queryData);
    QueryDataTab * dataTab = new QueryDataTab(queryData);
    _dataTabs->addTab(dataTab, _presenter->resultTabCaption(queryIndex));
}

void QueryResult::removeAllDataTabs()
{
    for (int i=0; i < _dataTabs->count(); ++i) {
        QWidget * tabWidget = _dataTabs->widget(i);
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);
        delete dataTabWidget;
    }
    _dataTabs->clear();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
