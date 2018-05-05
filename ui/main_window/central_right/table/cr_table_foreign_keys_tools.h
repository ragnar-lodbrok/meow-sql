#ifndef UI_CENTRAL_RIGHT_TABLE_FOREIGN_KEYS_TOOLS_H
#define UI_CENTRAL_RIGHT_TABLE_FOREIGN_KEYS_TOOLS_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

class ForeignKeysTab;

class TableForeignKeysTools : public QWidget
{
    Q_OBJECT
public:
    explicit TableForeignKeysTools(ForeignKeysTab * parent);

    enum class Action {
        Add,
        Remove,
        Clear
    };

    void setActionEnabled(Action action, bool enabled);

private:

    void createWidgets();

    QToolBar * _toolBar;

    QAction * _addAction;
    QAction * _removeAction;
    QAction * _clearAction;

    ForeignKeysTab * _parent;
};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_FOREIGN_KEYS_TOOLS_H
