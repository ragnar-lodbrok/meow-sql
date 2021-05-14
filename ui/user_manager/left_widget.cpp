#include "left_widget.h"
#include "window.h"
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
    , _tableModel(this)
    , _form(form)
    , _window(window)
{
    createWidgets();

    connect(_form, &models::forms::UserManagementForm::selectedUserChanged,
            this, &LeftWidget::onSelectedUserChanged);
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

    _proxyTableModel.setSourceModel(&_tableModel);
    _userList->setModel(&_proxyTableModel);
    mainLayout->addWidget(_userList);
    _userList->setSortingEnabled(true);
    _userList->sortByColumn(
                (int)models::db::UsersTableModel::Columns::Username,
                Qt::AscendingOrder);
    _userList->setSelectionBehavior(
                QAbstractItemView::SelectionBehavior::SelectRows);
    _userList->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i=0; i < _tableModel.columnCount(); ++i) {
        _userList->setColumnWidth(i, _tableModel.columnWidth(i));
    }

    connect(_userList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &LeftWidget::userListSelectionChanged
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
            &LeftWidget::onAddUserClicked
    );

    _cloneUserButton = new QPushButton(QIcon(":/icons/page_copy.png"),
                                       tr("Clone"));
    buttonsLayout->addWidget(_cloneUserButton);
    connect(_cloneUserButton,
            &QAbstractButton::clicked,
            this,
            &LeftWidget::onCloneUserClicked
    );

    _deleteUserButton = new QPushButton(QIcon(":/icons/delete.png"),
                                        tr("Delete"));
    buttonsLayout->addWidget(_deleteUserButton);
    connect(_deleteUserButton,
            &QAbstractButton::clicked,
            this,
            &LeftWidget::onDeleteUserClicked
    );

    this->setLayout(mainLayout);
}

void LeftWidget::loadData()
{
    _form->userManager()->refresh();
    _tableModel.setUserManager(_form->userManager());
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

    Q_UNUSED(deselected);

    try {

        QModelIndex index;
        QModelIndexList items = selected.indexes();

        if (!items.isEmpty()) {
            index = items.at(0);
            QModelIndex realIndex = _proxyTableModel.mapToSource(index);

            const meow::db::UserPtr & user
                    = _form->userManager()->userList().value(realIndex.row());
            _form->selectUser(user);

        } else {
            _form->selectUser(nullptr);
        }

    } catch (const meow::db::Exception & ex) {
        _window->showErrorMessage(ex.message());
    }

}

void LeftWidget::onSelectedUserChanged()
{

}

void LeftWidget::onAddUserClicked()
{

}

void LeftWidget::onCloneUserClicked()
{

}

void LeftWidget::onDeleteUserClicked()
{

}

} // namespace user_manager
} // namespace ui
} // namespace
