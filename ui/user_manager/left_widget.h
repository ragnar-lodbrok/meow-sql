#ifndef UI_USER_MANAGER_LEFT_WIDGET_H
#define UI_USER_MANAGER_LEFT_WIDGET_H

#include <QtWidgets>
#include "ui/presenters/user_management_form.h"

namespace meow {
namespace ui {
namespace user_manager {

class Window;

class LeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftWidget(presenters::UserManagementForm * form,
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

    Q_SLOT void selectRow(const QModelIndex & index);
    Q_SLOT void onSelectRowQueued(const QModelIndex & index);
    Q_SIGNAL void selectRowQueued(const QModelIndex & index);

    Q_SLOT void connectUserListSelectionChanged();
    Q_SLOT void disconnectUserListSelectionChanged();

    QSortFilterProxyModel _proxyTableModel;
    presenters::UserManagementForm * _form;

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
