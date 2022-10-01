#ifndef UI_CENTRAL_RIGHT_QUERY_TAB_H
#define UI_CENTRAL_RIGHT_QUERY_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/main_window/central_right/global_data_filter_interface.h"
#include "ui/presenters/central_right_query_presenter.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class QueryPanel;
class QueryResult;

class QueryTab : public BaseRootTab, public IGlobalDataFilter
{
    Q_OBJECT
public:
    explicit QueryTab(db::UserQuery * query, QWidget *parent = nullptr);
    ~QueryTab();

    QString currentQueryText() const;
    void setCurrentQueryText(const QString & text);

    virtual void setFilterPattern(const QString & filter,
                                  bool regexp = false) override;
    virtual QString filterPattern() const override;
    virtual bool filterPatternIsRegexp() const override;

    virtual int totalRowCount() const override;
    virtual int filterMatchedRowCount() const override;

    Q_SIGNAL void queryResultTabChanged(int index);

private:

    void saveGeometryToSettings();
    void loadGeometryFromSettings();

    void createWidgets();
    void validateControls();

    Q_SLOT void onActionExecQuery();
    Q_SLOT void onActionExecCurrentQuery(int charPosition);
    Q_SLOT void onActionCancelQuery();
    Q_SLOT void onExecQueriesFinished();
    Q_SLOT void onExecQueryFinished(int queryIndex, int totalCount);
    Q_SLOT void onExecQueryDataResult(int queryIndex);
    Q_SLOT void onExecQueriesRunningChanged();
    Q_SLOT void onExecutionConnectionClosed();

    void beforeRunQueries();

    QHBoxLayout * _mainLayout;
    QSplitter * _mainVerticalSplitter;

    QueryPanel * _queryPanel;
    QueryResult * _queryResult;
    
    presenters::CentralRightQueryPresenter _presenter;
};

class AddQueryTab : public BaseRootTab
{
public:
    explicit AddQueryTab(QWidget * parent = nullptr);
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_TAB_H
