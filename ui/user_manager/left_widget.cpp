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
    //_tableModel.setUserManager(form->userManager());
    createWidgets();

    connect(_form, &models::forms::UserManagementForm::selectedUserChanged,
            this, &LeftWidget::onSelectedUserChanged);
}

void LeftWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    _usersLabel = new QLabel(tr("Select user account:"));
    mainLayout->addWidget(_usersLabel);

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

    this->setLayout(mainLayout);
}

void LeftWidget::loadData()
{
     _tableModel.setUserManager(_form->userManager());
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

} // namespace user_manager
} // namespace ui
} // namespace
