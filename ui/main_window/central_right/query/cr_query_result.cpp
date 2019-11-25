#include "cr_query_result.h"
#include "db/user_query/user_query.h"
#include "cr_query_data_tab.h"
#include "helpers/formatting.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryResult::QueryResult(db::UserQuery * userQuery, QWidget *parent)
    : QWidget(parent),
      _userQuery(userQuery)
{

    QVBoxLayout * layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    _dataTabs = new QTabWidget();
    layout->addWidget(_dataTabs);

    _dataTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    #ifdef Q_OS_MAC
        _dataTabs->setDocumentMode(true);
    #endif

}

QueryResult::~QueryResult()
{
    removeAllDataTabs();
}

void QueryResult::showQueryData()
{
    removeAllDataTabs();
    int resultsCount = _userQuery->resultsDataCount();
    for (int i=0; i<resultsCount; ++i) {
        QueryDataTab * dataTab = new QueryDataTab(_userQuery->resultsDataAt(i));
        _dataTabs->addTab(dataTab, dataTabCaption(i));
    }
}

QString QueryResult::dataTabCaption(int index) const
{
    meow::db::QueryData * queryData = _userQuery->resultsDataAt(index);
    QString caption =  QObject::tr("Result") + " #" + QString::number(index+1);
    caption += QString(" (%1×%2)")
            .arg(helpers::formatNumber(queryData->columnCount()))
            .arg(helpers::formatNumber(queryData->rowCount()));
    return caption;
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
