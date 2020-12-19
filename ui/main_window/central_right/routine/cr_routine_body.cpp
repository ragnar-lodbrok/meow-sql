#include "cr_routine_body.h"
#include "models/forms/routine_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

RoutineBody::RoutineBody(meow::models::forms::RoutineForm * form,
                         QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}

void RoutineBody::refreshData()
{
    _bodyEdit->setPlainText(_form->body());
    _bodyEdit->setReadOnly(!_form->isEditingSupported());
}

void RoutineBody::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();

    _bodyLabel = new QLabel(tr("Routine body:"));
    mainLayout->addWidget(_bodyLabel);

    _bodyEdit = new ui::common::SQLEditor();
    connect(_bodyEdit, &QPlainTextEdit::textChanged,
            [=]() {
                _form->setHasUnsavedChanges(true);
                // don't copy each time as can be long
            });
    mainLayout->addWidget(_bodyEdit);

    setLayout(mainLayout);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
