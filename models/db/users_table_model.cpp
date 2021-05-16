#include "users_table_model.h"
#include "db/user_manager.h"

namespace meow {
namespace models {
namespace db {

UsersTableModel::UsersTableModel(QObject * parent)
    : QAbstractTableModel(parent)
    , _userManager(nullptr)
    , _rowCount(0)
{

}

Qt::ItemFlags UsersTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    return flags;
}

QVariant UsersTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {

        case Columns::Username:
            return QString(tr("Username"));

        case Columns::Host:
            return QString(tr("Host"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant UsersTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        const QList<meow::db::UserPtr> & users = _userManager->userList();
        const meow::db::UserPtr & user = users.at(index.row());

        switch (static_cast<Columns>(index.column())) {

        case Columns::Username:
            return user->username();

        case Columns::Host:
            return user->host();

        default:
            break;
        }
    }

    return QVariant();
}

int UsersTableModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int UsersTableModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return _rowCount;
}

int UsersTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {

    case Columns::Username:
        return 145;
    case Columns::Host:
        return 145;

    default:
        break;
    }
    return 0;
}

void UsersTableModel::setUserManager(meow::db::IUserManager * userManager)
{
    removeAllRows();
    if (_userManager) {
        disconnect(_userManager,
                &meow::db::IUserManager::userDataChanged,
                this,
                &UsersTableModel::onUserDataChanged);
    }
    _userManager = userManager;
    if (_userManager) {
        connect(_userManager,
                &meow::db::IUserManager::userDataChanged,
                this,
                &UsersTableModel::onUserDataChanged);
    }
    insertAllRows();
}

QModelIndex UsersTableModel::appendEmptyUser()
{

    if (!_userManager) return QModelIndex();

    meow::db::UserPtr newUser = meow::db::createUserPtr();
    newUser->setIsNew(true);
    newUser->setHost("localhost");
    newUser->setUsername("unnamed");
    newUser->privileges() << std::make_shared<meow::db::UserPrivilege>(
                              meow::db::UserPrivilege::Scope::Global);

    return appendUser(newUser);
}

QModelIndex UsersTableModel::cloneAndAppendUser(const meow::db::UserPtr & user)
{
    if (!_userManager) return QModelIndex();

    meow::db::UserPtr newUser = meow::db::createUserPtr();
    newUser->copyDataFrom(user.get());
    newUser->setIsNew(true);

    return appendUser(newUser);
}

bool UsersTableModel::deleteUser(const meow::db::UserPtr & user)
{
    if (!_userManager) return false;

    int removeRow = _userManager->userList().indexOf(user);
    if (removeRow == -1) return false;

    beginRemoveRows(QModelIndex(), removeRow, removeRow);
    _userManager->deleteUser(user);
    --_rowCount;
    endRemoveRows();

    return true;
}

void UsersTableModel::removeAllRows()
{
    if (_rowCount) {
        beginRemoveRows(QModelIndex(), 0, _rowCount-1);
        _rowCount = 0;
        endRemoveRows();
    }
}

void UsersTableModel::insertAllRows()
{
    int userCount = this->userCount();
    if (userCount) {
        beginInsertRows(QModelIndex(), 0, userCount-1);
        _rowCount = userCount;
        endInsertRows();
    }
}

int UsersTableModel::userCount() const
{
    if (_userManager) {
        return _userManager->userList().count();
    }
    return 0;
}

QModelIndex UsersTableModel::appendUser(const meow::db::UserPtr & user)
{
    int insertRow = rowCount();

    beginInsertRows(QModelIndex(), insertRow, insertRow);
    _userManager->addUser(user);
    ++_rowCount;
    endInsertRows();

    onUserDataChanged(user);

    return this->index(insertRow, static_cast<int>(Columns::Username));
}

void UsersTableModel::onUserDataChanged(const meow::db::UserPtr & user)
{
    if (_userManager) {
        const QList<meow::db::UserPtr> & userList = _userManager->userList();
        int userIndex = userList.indexOf(user);
        if (userIndex != -1) {
            emit dataChanged(
                createIndex(userIndex, static_cast<int>(Columns::Username)),
                createIndex(userIndex, static_cast<int>(Columns::Host))
            );
        }
    }
}

} // namespace db
} // namespace models
} // namespace meow
