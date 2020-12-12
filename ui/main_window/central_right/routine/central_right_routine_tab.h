#ifndef UI_CENTRAL_RIGHT_ROUTINE_TAB_H
#define UI_CENTRAL_RIGHT_ROUTINE_TAB_H

#include <QtWidgets>
#include "models/forms/routine_form.h"

namespace meow {

namespace db {
    class RoutineEntity;
}

namespace ui {
namespace main_window {
namespace central_right {

class RoutineInfo;
class RoutineBody;

class RoutineTab : public QWidget
{
public:
    explicit RoutineTab(QWidget * parent = nullptr);

    void setRoutine(meow::db::RoutineEntity * routine);

private:

    void createWidgets();

    models::forms::RoutineForm _form;

    QSplitter * _mainSplitter;

    RoutineInfo * _info;
    RoutineBody * _body;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_TAB_H
