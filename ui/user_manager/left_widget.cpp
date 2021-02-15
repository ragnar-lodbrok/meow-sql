#include "left_widget.h"
#include "app/app.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"

namespace meow {
namespace ui {
namespace user_manager {

LeftWidget::LeftWidget(db::SessionEntity *session,
                       QWidget *parent)
    : QWidget(parent)
    , _tableModel(this)
{
    _tableModel.setUserManager(session->connection()->userManager());
    createWidgets();
}

void LeftWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();

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

    /*connect(_userList->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            this,
            &Window::currentSessionDetailsChanged
    );*/

    this->setLayout(mainLayout);
}

} // namespace user_manager
} // namespace ui
} // namespace
