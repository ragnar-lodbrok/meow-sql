#ifndef UI_CENTRAL_RIGHT_QUERY_PANEL_H
#define UI_CENTRAL_RIGHT_QUERY_PANEL_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace common {
class SQLEditor;
}

namespace main_window {
namespace central_right {

class QueryTab;

class QueryPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QueryPanel(QueryTab * queryTab);

    QString queryPlainText() const;
    
private:

    void createWidgets();
    void createToolBar();

    QueryTab * _queryTab;
    
    QHBoxLayout * _mainLayout;
    ui::common::SQLEditor * _queryTextEdit;

    QToolBar * _toolBar;
    QAction * _runAction;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_PANEL_H
