#ifndef UI_CENTRAL_RIGHT_QUERY_RESULT_H
#define UI_CENTRAL_RIGHT_QUERY_RESULT_H

#include <QtWidgets>

namespace meow {

namespace db {
    class UserQuery;
}

namespace ui {
namespace main_window {
namespace central_right {

class QueryResult : public QWidget
{
    Q_OBJECT
public:
    explicit QueryResult(db::UserQuery * userQuery, QWidget *parent = 0);
    ~QueryResult();

    void showQueryData();

private:

    QString dataTabCaption(int index) const;
    void removeAllDataTabs();

    db::UserQuery * _userQuery;

    QTabWidget  * _dataTabs;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_RESULT_H
