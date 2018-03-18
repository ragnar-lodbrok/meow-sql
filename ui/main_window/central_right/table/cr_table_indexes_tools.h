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

private:

    void createWidgets();

    QToolBar * _toolBar;

    QAction * _addIndexAction;
    QAction * _removeIndexAction;
    QAction * _clearIndexesAction;
    QAction * _moveUpIndexAction;
    QAction * _moveDownIndexAction;

    IndexesTab * _parent;
};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INDEXES_TOOLS_H
