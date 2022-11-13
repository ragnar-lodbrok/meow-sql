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
    connect(_dataTabs,
            &QTabWidget::currentChanged,
            this,
            &QueryResult::onQueryDataTabChanged);
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

void QueryResult::showQueryData(int queryResultIndex)
{
    db::QueryDataPtr queryData = _presenter->resultsDataAt(queryResultIndex);
    Q_ASSERT(queryData);
    QueryDataTab * dataTab = new QueryDataTab(queryData);
    _dataTabs->addTab(dataTab, _presenter->resultTabCaption(queryResultIndex));
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

void QueryResult::setFilterPattern(const QString & filter, bool regexp)
{
    _presenter->setFilterPattern(filter, regexp);

    QWidget * tabWidget = _dataTabs->currentWidget();
    if (tabWidget) {
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);

        dataTabWidget->setFilterPattern(
            filterPattern(),
            filterPatternIsRegexp());
    }
}

QString QueryResult::filterPattern() const
{
    return _presenter->filterPattern();
}

bool QueryResult::filterPatternIsRegexp() const
{
    return _presenter->filterPatternIsRegexp();
}

int QueryResult::totalRowCount() const
{
    QWidget * tabWidget = _dataTabs->currentWidget();
    if (tabWidget) {
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);
        return dataTabWidget->totalRowCount();
    }
    return 0;
}

int QueryResult::filterMatchedRowCount() const
{
    QWidget * tabWidget = _dataTabs->currentWidget();
    if (tabWidget) {
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);
        return dataTabWidget->filterMatchedRowCount();
    }
    return 0;
}

void QueryResult::onQueryDataTabChanged(int index)
{
    QWidget * tabWidget = (index >= 0) ? _dataTabs->widget(index) : nullptr;
    if (tabWidget) {
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);

        dataTabWidget->setFilterPattern(
            filterPattern(),
            filterPatternIsRegexp());
    }

    emit queryDataTabChanged(index); // emit at the bottom!
}

void QueryResult::onDataExportAction()
{
    QWidget * tabWidget = _dataTabs->currentWidget();
    if (tabWidget) {
        QueryDataTab * dataTabWidget = static_cast<QueryDataTab *>(tabWidget);
        dataTabWidget->onDataExportAction();
    }
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
