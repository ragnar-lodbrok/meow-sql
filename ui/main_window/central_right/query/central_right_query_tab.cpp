#include "central_right_query_tab.h"
#include "cr_query_panel.h"
#include "cr_query_result.h"
#include "db/user_query/user_query.h"
#include "db/user_query/sentences_parser.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryTab::QueryTab(db::UserQuery * query, QWidget *parent) : 
    QWidget(parent),
    _query(query)
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

    _queryPanel = new QueryPanel(this);
    _queryPanel->setMinimumHeight(80);
    _mainVerticalSplitter->addWidget(_queryPanel);

    _queryResult = new QueryResult(_query);
    _queryResult->setMinimumHeight(80);
    _mainVerticalSplitter->addWidget(_queryResult);

    _mainVerticalSplitter->setSizes({150, 500});
}

void QueryTab::onActionRun(bool checked)
{
    Q_UNUSED(checked);
    meow::db::user_query::SentencesParser parser;
    QStringList queries = parser.parseByDelimiter(_queryPanel->queryPlainText());
    // Listening: Arch Enemy - On And On
    bool success = _query->runInCurrentConnection(queries);
    _queryResult->showQueryData();
    if (!success) {
        QMessageBox msgBox;
        msgBox.setText(_query->lastError());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
