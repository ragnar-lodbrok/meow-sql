#ifndef UI_USER_MANAGER_PRIVILEGES_WIDGET_H
#define UI_USER_MANAGER_PRIVILEGES_WIDGET_H

#include <QtWidgets>
#include "ui/models/user_privileges_model.h"

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models

namespace ui {
namespace user_manager {

class Window;

class PrivilegesWidget : public QWidget
{
public:
    explicit PrivilegesWidget(presenters::UserManagementForm * form,
                              Window * window);
    void fillDataFromForm();
    void validateControls();
private:
    void createWidgets();

    Q_SLOT void onAddObjectClicked();

    models::UserPrivilegesModel _treeModel;

    presenters::UserManagementForm * _form;
    Window * _window;

    QLabel * _accessLabel;
    QPushButton * _addObjectButton;
    QTreeView * _privilegesTree;

};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_PRIVILEGES_WIDGET_H
