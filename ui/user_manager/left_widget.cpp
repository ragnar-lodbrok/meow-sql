#include "left_widget.h"
#include "user_manager_window.h"
#include "app/app.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace user_manager {

LeftWidget::LeftWidget(models::forms::UserManagementForm * form,
                       Window * window)
    : QWidget(window)
    , _form(form)
    , _window(window)
{
    createWidgets();
}

void LeftWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 4, 0);

    _usersLabel = new QLabel(tr("Select user account:"));
    mainLayout->addWidget(_usersLabel);

    // Table -------------------------------------------------------------------

    _userList = new QTableView();
    _userList->verticalHeader()->hide();
    _userList->horizontalHeader()->setHighlightSections(false);
    auto geometrySettings = meow::app()->settings()->geometrySettings();
    _userList->verticalHeader()->setDefaultSectionSize(
       geometrySettings->tableViewDefaultRowHeight());

    _proxyTableModel.setSourceModel(_form->tableModel());
    _userList->setModel(&_proxyTableModel);
    mainLayout->addWidget(_userList);
    _userList->setSortingEnabled(true);
    _userList->sortByColumn(
                (int)models::db::UsersTableModel::Columns::Username,
                Qt::AscendingOrder);
    _userList->setSelectionBehavior(
                QAbstractItemView::SelectionBehavior::SelectRows);
    _userList->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i=0; i < _form->tableModel()->columnCount(); ++i) {
        _userList->setColumnWidth(i, _form->tableModel()->columnWidth(i));
    }

    connectUserListSelectionChanged();

    connect(this,
            &LeftWidget::selectRowQueued,
            this,
            &LeftWidget::onSelectRowQueued,
            Qt::QueuedConnection
    );

    // Buttons -----------------------------------------------------------------

    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(buttonsLayout);

    _addUserButton = new QPushButton(QIcon(":/icons/add.png"),
                                     tr("Add"));
    buttonsLayout->addWidget(_addUserButton);
    connect(_addUserButton,
            &QAbstractButton::clicked,
            this,
            &LeftWidget::addUserClicked
    );

    _cloneUserButton = new QPushButton(QIcon(":/icons/page_copy.png"),
                                       tr("Clone"));
    buttonsLayout->addWidget(_cloneUserButton);
    connect(_cloneUserButton,
            &QAbstractButton::clicked,
            this,
            &LeftWidget::cloneUserClicked
    );

    _deleteUserButton = new QPushButton(QIcon(":/icons/delete.png"),
                                        tr("Delete"));
    buttonsLayout->addWidget(_deleteUserButton);
    connect(_deleteUserButton,
            &QAbstractButton::clicked,
            this,
            &LeftWidget::deleteUserClicked
    );

    this->setLayout(mainLayout);
}

void LeftWidget::validateControls()
{
    _addUserButton->setEnabled(_form->canAddUser());
    _cloneUserButton->setEnabled(_form->canCloneSelectedUser());
    _deleteUserButton->setEnabled(_form->canDeleteSelectedUser());
}

void LeftWidget::userListSelectionChanged(const QItemSelection &selected,
                                          const QItemSelection &deselected)
{
    {
        QModelIndexList deselectedItems = deselected.indexes();
        if (!deselectedItems.isEmpty()) {
            QModelIndex deselectedIndex = _proxyTableModel.mapToSource(
                            deselectedItems.at(0));

            const meow::db::UserPtr & deselectedUser
                    = _form->userManager()->userList().value(
                        deselectedIndex.row());

            bool isLeavingCurrentEdit = false;

            if (_form->isNewUser()) {
                isLeavingCurrentEdit = deselectedUser == _form->selectedUser();
            } else {
                isLeavingCurrentEdit = deselectedUser == _form->sourceUser();
            }

            if (deselectedUser
                    && isLeavingCurrentEdit
                    && _form->hasUnsavedChanges()) {

                QString confirmMsg = QObject::tr("Save modified user?");

                QMessageBox msgBox;
                msgBox.setText(confirmMsg);
                msgBox.setStandardButtons(
                            QMessageBox::Yes
                          | QMessageBox::No
                          | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Yes);
                msgBox.setIcon(QMessageBox::Question);
                int ret = msgBox.exec();
                if (ret == QMessageBox::Yes) {
                    try {
                        _form->save();
                    } catch(meow::db::Exception & ex) {
                        _window->showErrorMessage(ex.message());
                        emit selectRowQueued(deselectedIndex);
                        return; // don't allow row change on error
                    }

                } else if (ret == QMessageBox::No) {
                    _form->discard(true); // new user only
                    // allow change
                } else if (ret == QMessageBox::Cancel) {
                    emit selectRowQueued(deselectedIndex);
                    return; // don't allow
                }
            }
        }
    }

    try {

        QModelIndex index;
        QModelIndexList items = selected.indexes();

        if (!items.isEmpty()) {
            index = items.at(0);
            QModelIndex realIndex = _proxyTableModel.mapToSource(index);

            const meow::db::UserPtr & user
                    = _form->userManager()->userList().value(realIndex.row());                         
            if (user != _form->sourceUser()) { // check to allow to revert
                                            // selection w/o loosing edited data
                _form->selectUser(user);
            }

        } else {
            _form->selectUser(nullptr);
        }

    } catch (const meow::db::Exception & ex) {
        _window->showErrorMessage(ex.message());
    }

}

void LeftWidget::onAddUserClicked()
{
    QModelIndex newUserIndex = _form->tableModel()->appendEmptyUser();
    selectRow(newUserIndex);
}

void LeftWidget::onCloneUserClicked()
{
    if (!_form->sourceUser()) return;
    QModelIndex clonedUserIndex = _form->tableModel()->cloneAndAppendUser(
                _form->sourceUser());
    selectRow(clonedUserIndex);
}

void LeftWidget::selectRow(const QModelIndex & index)
{
    if (!index.isValid()) return;

    QModelIndex realIndex = _proxyTableModel.mapFromSource(index);

    // select
    _userList->selectionModel()->select(
            realIndex,
            QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    _userList->selectionModel()->setCurrentIndex(
            realIndex,
            QItemSelectionModel::Current);

    // scroll
    _userList->scrollTo(_userList->selectionModel()->currentIndex());
}

void LeftWidget::onSelectRowQueued(const QModelIndex & index)
{
    //disconnectUserListSelectionChanged();
    selectRow(index);
    //connectUserListSelectionChanged();
}

void LeftWidget::connectUserListSelectionChanged()
{
    connect(_userList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &LeftWidget::userListSelectionChanged
    );
}
void LeftWidget::disconnectUserListSelectionChanged()
{
    disconnect(_userList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &LeftWidget::userListSelectionChanged
    );
}

} // namespace user_manager
} // namespace ui
} // namespace
