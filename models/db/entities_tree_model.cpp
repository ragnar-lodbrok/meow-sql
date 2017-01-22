#include "entities_tree_model.h"

namespace meow {
namespace models {
namespace db {

EntitiesTreeModel::EntitiesTreeModel(
        meow::db::ConnectionsManager * dbConnectionsManager,
        QObject * parent)
    :QAbstractItemModel(parent),
     _dbConnectionsManager(dbConnectionsManager)
{
    Q_UNUSED(_dbConnectionsManager);
}

Qt::ItemFlags EntitiesTreeModel::flags(const QModelIndex &index) const // override
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QModelIndex EntitiesTreeModel::index(int row,
                             int column,
                             const QModelIndex &parent) const // override
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex EntitiesTreeModel::parent(const QModelIndex &index) const // override
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    return QModelIndex();
}

int EntitiesTreeModel::rowCount(const QModelIndex &parent) const // override
{
;
    if (parent.column() > 0) {
        return 0;
    }

    return 1;
}

int EntitiesTreeModel::columnCount(const QModelIndex &parent) const // override
{
    if (parent.isValid()) {
        return 1; // TODO: add size column later
    } else {
        return 0;
    }
}

QVariant EntitiesTreeModel::data(const QModelIndex &index, int role) const // override
{


    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    return QVariant(QString("test session name"));
}

} // namespace db
} // namespace models
} // namespace meow
