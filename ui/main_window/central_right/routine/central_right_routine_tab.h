#ifndef UI_CENTRAL_RIGHT_ROUTINE_TAB_H
#define UI_CENTRAL_RIGHT_ROUTINE_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/presenters/routine_form.h"

namespace meow {

namespace db {
    class RoutineEntity;
}

namespace ui {
namespace main_window {
namespace central_right {

class RoutineInfo;
class RoutineBody;

class RoutineTab : public BaseRootTab
{
public:
    explicit RoutineTab(QWidget * parent = nullptr);

    void setRoutine(const meow::db::RoutineEntityPtr & routine);

    void onBeforeEntityEditing();

private:

    void createWidgets();
    void createGeneralButtons();

    Q_SLOT void discardEditing();
    Q_SLOT void saveEditing();

    Q_SLOT void validateControls();

    presenters::RoutineForm _form;

    QSplitter * _mainSplitter;

    RoutineInfo * _info;
    RoutineBody * _body;

    QPushButton * _discardButton;
    QPushButton * _saveButton;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ROUTINE_TAB_H
