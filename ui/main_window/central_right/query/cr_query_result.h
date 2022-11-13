#ifndef UI_CENTRAL_RIGHT_QUERY_RESULT_H
#define UI_CENTRAL_RIGHT_QUERY_RESULT_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
class CentralRightQueryPresenter;
}

namespace main_window {
namespace central_right {

// TODO: rename to QueryResultWidget to distinguish from db
class QueryResult : public QWidget
{
    Q_OBJECT
public:
    explicit QueryResult(presenters::CentralRightQueryPresenter * presenter,
                         QWidget *parent = 0);
    ~QueryResult();

    void showAllQueriesData();
    void hideAllQueriesData();

    void showQueryData(int queryResultIndex);

    void setFilterPattern(const QString & filter,
                                  bool regexp = false);
    QString filterPattern() const;
    bool filterPatternIsRegexp() const;

    int totalRowCount() const;
    int filterMatchedRowCount() const;

    Q_SIGNAL void queryDataTabChanged(int index);

    void onDataExportAction();

private:

    void removeAllDataTabs();

    Q_SLOT void onQueryDataTabChanged(int index);

    presenters::CentralRightQueryPresenter * _presenter;

    QTabWidget * _dataTabs;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_RESULT_H
