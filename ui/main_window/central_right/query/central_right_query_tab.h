#ifndef UI_CENTRAL_RIGHT_QUERY_TAB_H
#define UI_CENTRAL_RIGHT_QUERY_TAB_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class QueryPanel;
class QueryResult;

class QueryTab : public QWidget
{
    Q_OBJECT
public:
    explicit QueryTab(QWidget *parent = 0);

private:

    void createWidgets();

    QHBoxLayout * _mainLayout;
    QSplitter * _mainVerticalSplitter;

    QueryPanel * _queryPanel;
    QueryResult * _queryResult;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_TAB_H
