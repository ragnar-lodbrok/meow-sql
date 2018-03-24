#ifndef UI_CENTRAL_RIGHT_TABLE_INDEXES_TOOLS_H
#define UI_CENTRAL_RIGHT_TABLE_INDEXES_TOOLS_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

class IndexesTab;

class TableIndexesTools : public QWidget
{
    Q_OBJECT
public:
    explicit TableIndexesTools(IndexesTab * parent);

    enum class Action {
        AddIndex,
        AddColumn,
        Remove,
        Clear,
        MoveUp,
        MoveDown
    };

    void setActionEnabled(Action action, bool enabled);

private:

    void createWidgets();

    QToolBar * _toolBar;

    QAction * _addIndexAction;
    QAction * _addColumnAction;
    QAction * _removeAction;
    QAction * _clearIndexesAction;
    QAction * _moveUpColumnAction;
    QAction * _moveDownColumnAction;

    IndexesTab * _parent;
};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INDEXES_TOOLS_H
