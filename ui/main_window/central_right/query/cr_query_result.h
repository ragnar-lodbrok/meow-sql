#ifndef UI_CENTRAL_RIGHT_QUERY_RESULT_H
#define UI_CENTRAL_RIGHT_QUERY_RESULT_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace ui {

class CentralRightQueryPresenter;

}
}

namespace ui {
namespace main_window {
namespace central_right {

// TODO: rename to QueryResultWidget to distinguish from db
class QueryResult : public QWidget
{
    Q_OBJECT
public:
    explicit QueryResult(models::ui::CentralRightQueryPresenter * presenter,
                         QWidget *parent = 0);
    ~QueryResult();

    void showAllQueriesData();
    void hideAllQueriesData();

    void showQueryData(int queryIndex);

private:

    void removeAllDataTabs();

    models::ui::CentralRightQueryPresenter * _presenter;

    QTabWidget  * _dataTabs;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_RESULT_H
