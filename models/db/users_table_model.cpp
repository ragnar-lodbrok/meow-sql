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
        return 140;
    case Columns::Host:
        return 140;

    default:
        break;
    }
    return 0;
}

void UsersTableModel::setUserManager(meow::db::IUserManager * userManager)
{
    removeAllRows();
    _userManager = userManager;
    insertAllRows();
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


} // namespace db
} // namespace models
} // namespace meow
