#ifndef UI_CENTRAL_RIGHT_TRIGGER_TAB_H
#define UI_CENTRAL_RIGHT_TRIGGER_TAB_H

#include <QtWidgets>
#include "models/forms/trigger_form.h"

namespace meow {

namespace db {
    class TriggerEntity;
}

namespace ui {
namespace main_window {
namespace central_right {

class TriggerOptions;
class TriggerBody;

class TriggerTab : public QWidget
{
public:
    explicit TriggerTab(QWidget * parent = nullptr);

    void setTrigger(db::TriggerEntity * trigger);

private:

    void createWidgets();
    void createGeneralButtons();

    Q_SLOT void discardEditing();
    Q_SLOT void saveEditing();

    Q_SLOT void validateControls();

    models::forms::TriggerForm _form;

    QSplitter * _mainSplitter;

    TriggerOptions * _options;
    TriggerBody * _body;

    QPushButton * _discardButton;
    QPushButton * _saveButton;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TRIGGER_TAB_H
