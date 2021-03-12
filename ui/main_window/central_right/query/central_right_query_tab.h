#ifndef UI_CENTRAL_RIGHT_QUERY_TAB_H
#define UI_CENTRAL_RIGHT_QUERY_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"

namespace meow {

namespace db {

class UserQuery;

}

namespace ui {
namespace main_window {
namespace central_right {

class QueryPanel;
class QueryResult;

class QueryTab : public BaseRootTab
{
    Q_OBJECT
public:
    explicit QueryTab(db::UserQuery * query, QWidget *parent = nullptr);
    ~QueryTab();

    Q_SLOT void onActionRun(bool checked);

private:

    void saveGeometryToSettings();
    void loadGeometryFromSettings();

    void createWidgets();

    QHBoxLayout * _mainLayout;
    QSplitter * _mainVerticalSplitter;

    QueryPanel * _queryPanel;
    QueryResult * _queryResult;
    
    db::UserQuery * _query;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_TAB_H
