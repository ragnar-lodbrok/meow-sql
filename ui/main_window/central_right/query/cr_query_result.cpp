#include "cr_query_result.h"
#include "db/user_query/user_query.h"
#include "cr_query_data_tab.h"

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
    return QObject::tr("Result") + " #" + QString::number(index);
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
