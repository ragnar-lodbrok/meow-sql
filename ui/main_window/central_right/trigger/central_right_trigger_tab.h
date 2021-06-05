#ifndef UI_CENTRAL_RIGHT_TRIGGER_TAB_H
#define UI_CENTRAL_RIGHT_TRIGGER_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "models/forms/trigger_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TriggerOptions;
class TriggerBody;

class TriggerTab : public BaseRootTab
{
public:
    explicit TriggerTab(QWidget * parent = nullptr);

    void setTrigger(const db::TriggerEntityPtr & trigger);

    void onBeforeEntityEditing();

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
