#include "cr_trigger_body.h"
#include "models/forms/trigger_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TriggerBody::TriggerBody(meow::models::forms::TriggerForm * form,
                         QWidget *parent)
    : QWidget(parent)
    , _form(form)
{
    createWidgets();
}

void TriggerBody::refreshData()
{
    _bodyEdit->blockSignals(true);
    _bodyEdit->setPlainText(_form->statement());
    _bodyEdit->blockSignals(false);
    _bodyEdit->setReadOnly(!_form->isEditingSupported());
}

void TriggerBody::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();

    _bodyLabel = new QLabel(tr(
        "Trigger statement: (e.g. \"SET NEW.columnA = TRIM(OLD.columnA)\""));
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
