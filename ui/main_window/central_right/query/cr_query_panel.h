#ifndef UI_CENTRAL_RIGHT_QUERY_PANEL_H
#define UI_CENTRAL_RIGHT_QUERY_PANEL_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class QueryPanel : public QWidget
{
    Q_OBJECT
public:
    explicit QueryPanel(QWidget *parent = 0);

private:

    void createWidgets();
    void createToolBar();

    Q_SLOT void onActionRun(bool checked);

    QHBoxLayout * _mainLayout;
    QTextEdit * _queryTextEdit;

    QToolBar * _toolBar;
    QAction * _runAction;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_PANEL_H
