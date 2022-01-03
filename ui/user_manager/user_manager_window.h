#ifndef UI_USER_MANAGER_WINDOW_H
#define UI_USER_MANAGER_WINDOW_H

#include <QtWidgets>
#include "ui/presenters/user_management_form.h"

namespace meow {

namespace db {
    class SessionEntity;
}

namespace ui {

namespace main_window {
    class Window;
}

namespace user_manager {

class LeftWidget;
class RightWidget;

class Window : public QDialog
{
public:
    Window(main_window::Window * mainWindow,
           db::SessionEntity * session);
    ~Window() override;

    void loadData();

    void showErrorMessage(const QString& message);

private:

    void createWidgets();
    Q_SLOT void validateControls();

    Q_SLOT void onSelectedUserChanged();

    Q_SLOT void onSaveClicked();
    Q_SLOT void onDiscardClicked();

    Q_SLOT void onAddUserClicked();
    Q_SLOT void onCloneUserClicked();
    Q_SLOT void onDeleteUserClicked();

    presenters::UserManagementForm _form;

    main_window::Window * _mainWindow;
    db::SessionEntity * _session;

    QSplitter * _splitter;
    LeftWidget * _leftWidget;
    RightWidget * _rightWidget;

    QLabel * _warningLabel;
    QPushButton * _saveButton;
    QPushButton * _discardButton;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_WINDOW_H
