#ifndef UI_USER_MANAGER_PRIVILEGES_WIDGET_H
#define UI_USER_MANAGER_PRIVILEGES_WIDGET_H

#include <QtWidgets>
#include "models/db/user_privileges_model.h"

namespace meow {

namespace models {
namespace forms {
    class UserManagementForm;
} // namespace forms
} // namespace models

namespace ui {
namespace user_manager {

class PrivilegesWidget : public QWidget
{
public:
    explicit PrivilegesWidget(models::forms::UserManagementForm * form,
                              QWidget * parent = nullptr);
    void fillDataFromForm();
private:
    void createWidgets();

    models::db::UserPrivilegesModel _treeModel;

    models::forms::UserManagementForm * _form;

    QLabel * _accessLabel;
    QTreeView * _privilegesTree;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_PRIVILEGES_WIDGET_H
