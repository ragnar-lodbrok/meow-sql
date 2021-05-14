#ifndef UI_USER_MANAGER_LEFT_WIDGET_H
#define UI_USER_MANAGER_LEFT_WIDGET_H

#include <QtWidgets>
#include "models/db/users_table_model.h"
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

class Window;

class LeftWidget : public QWidget
{
public:
    explicit LeftWidget(models::forms::UserManagementForm * form,
                        Window * window = nullptr);

    void loadData();
    void validateControls();
private:

    void createWidgets();

    Q_SLOT void userListSelectionChanged(const QItemSelection &selected,
                                         const QItemSelection &deselected);

    Q_SLOT void onSelectedUserChanged();

    Q_SLOT void onAddUserClicked();
    Q_SLOT void onCloneUserClicked();
    Q_SLOT void onDeleteUserClicked();

    models::db::UsersTableModel _tableModel;
    QSortFilterProxyModel _proxyTableModel;
    models::forms::UserManagementForm * _form;

    QLabel * _usersLabel;
    QPushButton * _addUserButton;
    QPushButton * _cloneUserButton;
    QPushButton * _deleteUserButton;
    QTableView * _userList;
    Window * _window;
};

} // namespace user_manager
} // namespace ui
} // namespace meow


#endif // UI_USER_MANAGER_LEFT_WIDGET_H
