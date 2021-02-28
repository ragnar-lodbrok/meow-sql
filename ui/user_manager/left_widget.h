#ifndef UI_USER_MANAGER_LEFT_WIDGET_H
#define UI_USER_MANAGER_LEFT_WIDGET_H

#include <QtWidgets>
#include "models/db/users_table_model.h"
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

class LeftWidget : public QWidget
{
public:
    explicit LeftWidget(models::forms::UserManagementForm * form,
                        QWidget *parent = nullptr);
private:

    void createWidgets();

    Q_SLOT void userListSelectionChanged(const QItemSelection &selected,
                                         const QItemSelection &deselected);

    Q_SLOT void onSelectedUserChanged();

    models::db::UsersTableModel _tableModel;
    QSortFilterProxyModel _proxyTableModel;
    models::forms::UserManagementForm * _form;

    QLabel * _usersLabel;
    QTableView * _userList;
};

} // namespace user_manager
} // namespace ui
} // namespace meow


#endif // UI_USER_MANAGER_LEFT_WIDGET_H
