#ifndef UI_CENTRAL_RIGHT_ROUTINE_PARAMETERS_TOOLS_H
#define UI_CENTRAL_RIGHT_ROUTINE_PARAMETERS_TOOLS_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace routine_info {

class ParametersTab;

class RoutineParametersTools : public QWidget
{
    Q_OBJECT
public:
    explicit RoutineParametersTools(ParametersTab * parent);

    enum class Action {
        Add,
        Remove,
        Clear,
        MoveUp,
        MoveDown
    };

    void setActionEnabled(Action action, bool enabled);
private:

    void createWidgets();

    QToolBar * _toolBar;

    QAction * _addAction;
    QAction * _removeAction;
    QAction * _clearAction;
    QAction * _moveUpAction;
    QAction * _moveDownAction;

    ParametersTab * _parent;    
};

} // namespace routine_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_PARAMETERS_TOOLS_H
