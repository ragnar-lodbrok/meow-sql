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
    void setQueryText(const QString & text);

    Q_SIGNAL void execQueryRequested();
    Q_SIGNAL void execCurrentQueryRequested(int charPosition);
    Q_SIGNAL void cancelQueryRequested();
    
    QAction * execQueryAction() const {
        return _execQueryAction;
    }
    QAction * execCurrentQueryAction() const {
        return _execCurrentQueryAction;
    }
    QAction * cancelQueryAction() const {
        return _cancelQueryAction;
    }

private:

    void createWidgets();
    void createToolBar();

    Q_SLOT void onQueryTextEditContextMenu(const QPoint & pos);
    Q_SLOT void onExecCurrentQueryAction();

    QueryTab * _queryTab;
    
    QHBoxLayout * _mainLayout;
    ui::common::SQLEditor * _queryTextEdit;

    QToolBar * _toolBar;
    QAction * _execQueryAction;
    QAction * _execCurrentQueryAction;
    QAction * _cancelQueryAction;
    QAction * _separatorAction;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_PANEL_H
