#ifndef UI_PREFERENCES_DIALOG_H
#define UI_PREFERENCES_DIALOG_H

#include <QtWidgets>
#include "ui/presenters/preferences_presenter.h"

namespace meow {

namespace settings {
    class Core;
} // namespace settings

namespace ui {
namespace preferences {


class Dialog : public QDialog
{
    Q_OBJECT
public:
    Dialog(settings::Core * settings);
private:
    void createWidgets();
    void createTabs();
    void validateControls();

    Q_SLOT void onOkButtonClicked();
    Q_SLOT void onCancelButtonClicked();
    Q_SLOT void onApplyButtonClicked();
    Q_SLOT void onPresenterModifiedChanged();

    presenters::PreferencesPresenter _presenter;

    QTabWidget  * _rootTabs;

    QPushButton * _okButton;
    QPushButton * _cancelButton;
    QPushButton * _applyButton;
};

} // namespace preferences
} // namespace ui
} // namespace meow

#endif // UI_PREFERENCES_DIALOG_H
