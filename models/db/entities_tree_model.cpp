#include <QDebug>
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

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::connectionOpened,
            [=](meow::db::SessionEntity * newSession) {
                Q_UNUSED(newSession);
            });

}

Qt::ItemFlags EntitiesTreeModel::flags(const QModelIndex &index) const // override
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex EntitiesTreeModel::index(int row,
                             int column,
                             const QModelIndex &parent) const // override
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    meow::db::Entity * parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem = static_cast<meow::db::Entity *>(parent.internalPointer());
    }

    meow::db::Entity * childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex EntitiesTreeModel::parent(const QModelIndex &index) const // override
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    meow::db::Entity * childItem = static_cast<meow::db::Entity *>(index.internalPointer());
    meow::db::Entity * parentItem = childItem->parent();

    if (parentItem == rootItem()) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int EntitiesTreeModel::rowCount(const QModelIndex &parent) const // override
{
    meow::db::Entity * parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem = static_cast<meow::db::Entity *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int EntitiesTreeModel::columnCount(const QModelIndex &parent) const // override
{
    Q_UNUSED(parent);

    return 1; // TODO: add size column later
}

QVariant EntitiesTreeModel::data(const QModelIndex &index, int role) const // override
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::DecorationRole) {

        meow::db::Entity * item = static_cast<meow::db::Entity *>(index.internalPointer());

        if (role == Qt::DecorationRole) {
            return item->icon();
        } else if (role == Qt::DisplayRole) {
            return item->name();
        }
    }

    return QVariant();
}

} // namespace db
} // namespace models
} // namespace meow
