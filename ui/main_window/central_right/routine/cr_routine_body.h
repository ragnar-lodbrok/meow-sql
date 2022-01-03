#ifndef UI_CENTRAL_RIGHT_ROUTINE_BODY_H
#define UI_CENTRAL_RIGHT_ROUTINE_BODY_H

#include <QtWidgets>
#include "ui/common/sql_editor.h"

namespace meow {
namespace ui {

namespace presenters {
    class RoutineForm;
}

namespace main_window {
namespace central_right {

class RoutineBody : public QWidget
{
public:
    RoutineBody(presenters::RoutineForm * form,
                QWidget * parent = nullptr);

    void refreshData();

    QString bodyText() const {
        return _bodyEdit->toPlainText();
    }

private:

    void createWidgets();

    QLabel * _bodyLabel;
    ui::common::SQLEditor * _bodyEdit;

    presenters::RoutineForm * _form;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_BODY_H
