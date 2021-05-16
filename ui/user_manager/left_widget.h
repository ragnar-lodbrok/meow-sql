#ifndef UI_USER_MANAGER_LEFT_WIDGET_H
#define UI_USER_MANAGER_LEFT_WIDGET_H

#include <QtWidgets>
#include "models/forms/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

class Window;

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(models::forms::UserManagementForm * form,
                        Window * window = nullptr);

    void validateControls();

    Q_SIGNAL void addUserClicked();
    Q_SIGNAL void cloneUserClicked();
    Q_SIGNAL void deleteUserClicked();

    Q_SLOT void onAddUserClicked();
    Q_SLOT void onCloneUserClicked();

private:

    void createWidgets();

    Q_SLOT void userListSelectionChanged(const QItemSelection &selected,
                                         const QItemSelection &deselected);

    Q_SLOT void onSelectedUserChanged(); // rm?
    void selectRow(const QModelIndex & index);

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
