#ifndef UI_USER_MANAGER_LEFT_WIDGET_H
#define UI_USER_MANAGER_LEFT_WIDGET_H

#include <QtWidgets>
#include "models/db/users_table_model.h"

namespace meow {

namespace db {
    class SessionEntity;
}

namespace ui {
namespace user_manager {

class LeftWidget : public QWidget
{
public:
    explicit LeftWidget(db::SessionEntity * session,
                        QWidget *parent = nullptr);
private:

    void createWidgets();

    models::db::UsersTableModel _tableModel;
    QSortFilterProxyModel _proxyTableModel;

    QLabel * _usersLabel;
    QTableView  * _userList;
};

} // namespace user_manager
} // namespace ui
} // namespace


#endif // UI_USER_MANAGER_LEFT_WIDGET_H
