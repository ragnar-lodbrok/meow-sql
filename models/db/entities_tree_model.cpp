#include <QDebug>
#include "entities_tree_model.h"
#include "db/entity/database_entity.h"
#include "db/entity/table_entity.h"

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

                beginInsertRows(QModelIndex(),
                                newSession->row(), newSession->row());
                endInsertRows();
                //emit layoutChanged();
            });

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::entityEdited,
            this,
            &meow::models::db::EntitiesTreeModel::onEntityEdited);
    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::entityInserted,
            this,
            &meow::models::db::EntitiesTreeModel::onEntityInserted);

}

Qt::ItemFlags EntitiesTreeModel::flags(const QModelIndex &index) const
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

QModelIndex EntitiesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    meow::db::Entity * childItem
        = static_cast<meow::db::Entity *>(index.internalPointer());
    meow::db::Entity * parentItem = childItem->parent();

    if (parentItem == rootItem()) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int EntitiesTreeModel::rowCount(const QModelIndex &parent) const // override
{
    if (parent.column() > 0) {
        return 0;
    }

    meow::db::Entity * parentItem;

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

bool EntitiesTreeModel::hasChildren(const QModelIndex &parent) const // override
{
    if (parent.isValid()) {
        meow::db::Entity * parentItem = static_cast<meow::db::Entity *>(
                                            parent.internalPointer()
                                        );

        if (parentItem->type() == meow::db::Entity::Type::Session
            || parentItem->type() == meow::db::Entity::Type::Database) {
            return true;
        }
    }

    return QAbstractItemModel::hasChildren(parent);
}

QVariant EntitiesTreeModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole || role == Qt::DecorationRole) {

        meow::db::Entity * item
            = static_cast<meow::db::Entity *>(index.internalPointer());

        if (role == Qt::DecorationRole) {
            return item->icon();
        } else if (role == Qt::DisplayRole) {
            return item->name();
        }
    }

    return QVariant();
}

void EntitiesTreeModel::onSelectEntityAt(const QModelIndex &index)
{
    if (!index.isValid()) {
        qDebug() << "Tree: invalid index selected";
        return;
    }

    meow::db::Entity * selectedEntity
        = static_cast<meow::db::Entity *>(index.internalPointer());

    if (selectedEntity == nullptr) {
        qDebug() << "Tree: empty entity selected";
        return;
    }

    qDebug() << "Tree: selected item" << selectedEntity->name();

    selectedEntity->setWasSelected(true);
    _dbConnectionsManager->setActiveEntity(selectedEntity);

}

QModelIndex EntitiesTreeModel::indexForEntity(meow::db::Entity * entity)
{
    if (entity->type() == meow::db::Entity::Type::Session) {
        // TODO
    } else if (entity->type() == meow::db::Entity::Type::Database) {
        // TODO
    } else if ( (int)entity->type() >= (int)meow::db::Entity::Type::Table ) {

        meow::db::DataBaseEntity * parentEntity
          = static_cast<meow::db::EntityInDatabase *>(entity)->dataBaseEntity();
        if (!parentEntity) return QModelIndex();

        int entityIntIndex = parentEntity->indexOf(entity);
        if (entityIntIndex == -1) return QModelIndex();

        return createIndex(entityIntIndex, 0, entity);
    }

    return QModelIndex();
}

void EntitiesTreeModel::onEntityEdited(meow::db::Entity * entity)
{    
    // TODO: refresh entire database since renaming may change eg sorting?

    QModelIndex entityIndex = indexForEntity(entity);
    if (entityIndex.isValid()) {
        emit dataChanged(entityIndex, entityIndex);
    }
}

void EntitiesTreeModel::onEntityInserted(meow::db::Entity * entity)
{
    if (entity->type() == meow::db::Entity::Type::Session) {
        // TODO
    } else if (entity->type() == meow::db::Entity::Type::Database) {
        // TODO
    } else if ( (int)entity->type() >= (int)meow::db::Entity::Type::Table ) {
        meow::db::DataBaseEntity * parentEntity
          = static_cast<meow::db::EntityInDatabase *>(entity)->dataBaseEntity();
        int entityIntIndex = parentEntity->indexOf(entity);
        if (entityIntIndex == -1) return;
        QModelIndex parentIndex = createIndex(parentEntity->row(),
                                              0,
                                              parentEntity);

        beginInsertRows(parentIndex, entityIntIndex, entityIntIndex);
        endInsertRows();
    }
}

bool EntitiesTreeModel::canDropCurrentItem() const
{
    return isCurItemDatabaseOrLess();
}

bool EntitiesTreeModel::canInsertTableOnCurrentItem() const
{
    return isCurItemDatabaseOrLess();
}

void EntitiesTreeModel::createNewTable()
{
    _dbConnectionsManager->createEntity(meow::db::Entity::Type::Table);
}

bool EntitiesTreeModel::isCurItemDatabaseOrLess() const
{
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }
    return (int)curEntity->type() >= (int)meow::db::Entity::Type::Database;
}

} // namespace db
} // namespace models
} // namespace meow
