#include "central_right_query_tab.h"
#include "cr_query_panel.h"
#include "cr_query_result.h"
#include "db/user_query/user_query.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryTab::QueryTab(db::UserQuery * query, QWidget *parent) : 
    BaseRootTab(BaseRootTab::Type::Query, parent),
    _presenter(query)
{
    createWidgets();

    connect(_presenter.query(), &db::UserQuery::queriesFinished,
            this, &QueryTab::onExecQueriesFinished);

    connect(_presenter.query(), &db::UserQuery::queryFinished,
            this, &QueryTab::onExecQueryFinished);

    connect(_presenter.query(), &db::UserQuery::newQueryDataResult,
            this, &QueryTab::onExecQueryDataResult);

    connect(_presenter.query(), &db::UserQuery::isRunningChanged,
            this, &QueryTab::onExecQueriesRunningChanged);
}

QueryTab::~QueryTab()
{
    saveGeometryToSettings();
}

void QueryTab::saveGeometryToSettings()
{
    // TODO: no longer works, should not be here but in parent
    //QSettings settings;
    //settings.setValue(
    //    QString("ui/main_window/center_right/query_tab/%1/vsplitter")
    //            .arg(_query->uniqueId()),
    //    _mainVerticalSplitter->saveState());
}

void QueryTab::loadGeometryFromSettings()
{
    // TODO: no longer works
    //QSettings settings;
    //_mainVerticalSplitter->restoreState(
    //    settings.value(
    //        QString("ui/main_window/center_right/query_tab/%1/vsplitter")
    //            .arg(_query->uniqueId())
    //    ).toByteArray());
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

    connect(_queryPanel, &QueryPanel::execQueryRequested,
            this, &QueryTab::onActionExecQuery);

    connect(_queryPanel, &QueryPanel::execCurrentQueryRequested,
            this, &QueryTab::onActionExecCurrentQuery);

    connect(_queryPanel, &QueryPanel::cancelQueryRequested,
            this, &QueryTab::onActionCancelQuery);

    _queryResult = new QueryResult(&_presenter);
    _queryResult->setMinimumHeight(80);
    _mainVerticalSplitter->addWidget(_queryResult);

    _mainVerticalSplitter->setSizes({150, 500});

    loadGeometryFromSettings();

    validateControls();
}

void QueryTab::validateControls()
{
    _queryPanel->execQueryAction()->setEnabled(
                _presenter.isExecQueryActionEnabled());
    _queryPanel->execCurrentQueryAction()->setEnabled(
                _presenter.isExecCurrentQueryActionEnabled());
    _queryPanel->cancelQueryAction()->setEnabled(
                _presenter.isCancelQueryActionEnabled());
}

void QueryTab::onActionExecQuery()
{
    beforeRunQueries();
    _presenter.execQueries(_queryPanel->queryPlainText());

    // Listening: Arch Enemy - On And On
}

void QueryTab::onActionExecCurrentQuery(int charPosition)
{
    beforeRunQueries();
    _presenter.execQueries(_queryPanel->queryPlainText(), charPosition);
}

void QueryTab::onActionCancelQuery()
{
    if (!_presenter.cancelQueries()) {
        QMessageBox msgBox;
        msgBox.setText(_presenter.lastCancelError());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void QueryTab::onExecQueriesFinished()
{
    if (_presenter.hasError()) {
        QMessageBox msgBox;
        msgBox.setText(_presenter.lastError());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void QueryTab::onExecQueryFinished(int queryIndex, int totalCount)
{
    Q_UNUSED(queryIndex);
    Q_UNUSED(totalCount);
    // TODO: show progress
    //qDebug() << "Query finished: " << queryIndex+1 << "/" << totalCount;
}

void QueryTab::onExecQueryDataResult(int queryIndex)
{
    // Listening: Bolt Thrower - The Killchain
    _queryResult->showQueryData(queryIndex);
}

void QueryTab::onExecQueriesRunningChanged()
{
    if (_presenter.isRunning()) {
        this->setCursor(Qt::BusyCursor);
    } else {
        this->unsetCursor();
    }
    validateControls();
}

void QueryTab::beforeRunQueries()
{
    _queryResult->hideAllQueriesData();
}

QString QueryTab::currentQueryText() const
{
    return _queryPanel->queryPlainText();
}

void QueryTab::setCurrentQueryText(const QString & text)
{
    _queryPanel->setQueryText(text);
}

// -----------------------------------------------------------------------------

AddQueryTab::AddQueryTab(QWidget * parent) :
    BaseRootTab(BaseRootTab::Type::AddQuery, parent)
{

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
