#ifndef UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H
#define UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TableColumns;

// Intent: Tools widget for table columns widget
class TableColumnsTools : public QWidget
{
    Q_OBJECT
public:
    explicit TableColumnsTools(TableColumns * parent);

    enum class Action {
        Add,
        Remove,
        MoveUp,
        MoveDown
    };

    void setActionEnabled(Action action, bool enabled);

private:
    void createWidgets();

    QLabel * _columnsLabel;

    QToolBar * _toolBar;

    QAction * _addColumnAction;
    QAction * _removeColumnAction;
    QAction * _moveUpColumnAction;
    QAction * _moveDownColumnAction;

    TableColumns * _parent;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H
