#include "helpers/logger.h"
#include "entities_tree_model.h"
#include "db/entity/session_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/table_entity.h"
#include <QDebug>

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
            });

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::beforeConnectionClosed,
            [=](meow::db::SessionEntity * closedSession) {

                beginRemoveRows(QModelIndex(),
                                closedSession->row(), closedSession->row());
                endRemoveRows();
            });

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::entityEdited,
            this,
            &meow::models::db::EntitiesTreeModel::onEntityEdited);

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::entityInserted,
            this,
            &meow::models::db::EntitiesTreeModel::onEntityInserted);

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::activeDatabaseChanged,
            [=](const QString & database) {
                Q_UNUSED(database);
                onDatabasesDataChanged();
            });
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

int EntitiesTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1; // TODO: add size column later
}

bool EntitiesTreeModel::hasChildren(const QModelIndex &parent) const
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
        meowLogDebug() << "Tree: invalid index selected";
        return;
    }

    meow::db::Entity * selectedEntity
        = static_cast<meow::db::Entity *>(index.internalPointer());

    if (selectedEntity == nullptr) {
        meowLogDebug() << "Tree: empty entity selected";
        return;
    }

    meowLogDebug() << "Tree: selected item " << selectedEntity->name();

    selectedEntity->setWasSelected(true);
    _dbConnectionsManager->setActiveEntity(selectedEntity);

}

void EntitiesTreeModel::onEmptySelection()
{
    _dbConnectionsManager->setActiveEntity(nullptr);
}

QModelIndex EntitiesTreeModel::indexForEntity(meow::db::Entity * entity)
{
    if (entity->type() == meow::db::Entity::Type::Session) {
        // TODO: does it work?
        return createIndex(entity->row(), 0, entity);
    } else if (entity->type() == meow::db::Entity::Type::Database) {
        return createIndex(entity->row(), 0, entity);
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

meow::db::Entity * EntitiesTreeModel::currentEntity() const
{
    return _dbConnectionsManager->activeEntity();
}

meow::db::SessionEntity * EntitiesTreeModel::currentSession() const
{
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return nullptr;
    }

    return meow::db::sessionForEntity(curEntity);
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
        meow::db::DataBaseEntity * database
            = static_cast<meow::db::DataBaseEntity *>(entity);
        meow::db::SessionEntity * parentEntity = database->session();
        int entityIntIndex = parentEntity->indexOf(database);
        if (entityIntIndex == -1) return;
        QModelIndex parentIndex = createIndex(parentEntity->row(),
                                              0,
                                              parentEntity);

        beginInsertRows(parentIndex, entityIntIndex, entityIntIndex);
        endInsertRows();
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
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }

    return curEntity->connection()->features()->supportsDrop(curEntity->type());
}

bool EntitiesTreeModel::canCreateDatabaseOnCurrentItem() const
{
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }
    return curEntity->type() == meow::db::Entity::Type::Session
        && curEntity->connection()->features()
            ->supportsCreation(meow::db::Entity::Type::Database);
}

bool EntitiesTreeModel::canCreateEntityOnCurrentItem(
        meow::db::Entity::Type type) const
{
    if (type == meow::db::Entity::Type::Database) {
        return canCreateDatabaseOnCurrentItem();
    }

    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }

    return isCurItemDatabaseOrLess()
        && curEntity->connection()->features()->supportsCreation(type);
}

bool EntitiesTreeModel::allowUserManager() const
{
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }

    return curEntity->connection()->features()->supportsUserManagement();
}

void EntitiesTreeModel::createNewEntity(meow::db::Entity::Type type)
{
    _dbConnectionsManager->createEntity(type);
}

void EntitiesTreeModel::refreshActiveSession()
{
    // TODO: use dataChanged and invalidate only active session
    beginResetModel();
    _dbConnectionsManager->refreshActiveSession();
    endResetModel();
}

void EntitiesTreeModel::dropCurrentItem()
{
    meow::db::Entity * curEntity = currentEntity();
    if (!curEntity) return;

    dropEntity(curEntity);
}

void EntitiesTreeModel::dropEntity(meow::db::Entity * entity)
{
    QModelIndex curIndex = indexForEntity(entity);
    if (!curIndex.isValid()) return;

    if (_dbConnectionsManager->dropEntity(entity)) {
        beginRemoveRows(parent(curIndex), curIndex.row(), curIndex.row());
        _dbConnectionsManager->activeSession()->removeEntity(entity);
        endRemoveRows();
    }
}

bool EntitiesTreeModel::isCurItemDatabaseOrLess() const
{
    meow::db::Entity * curEntity = _dbConnectionsManager->activeEntity();
    if (!curEntity) {
        return false;
    }
    return (int)curEntity->type() >= (int)meow::db::Entity::Type::Database;
}

void EntitiesTreeModel::onDatabasesDataChanged()
{
    for (meow::db::SessionEntity * session : _dbConnectionsManager->sessions()) {
        for (meow::db::DataBaseEntity * dbEntity : session->databases()) {
            QModelIndex dbEntityIndex = indexForEntity(dbEntity);
            emit dataChanged(dbEntityIndex, dbEntityIndex);
        }
    }
}

} // namespace db
} // namespace models
} // namespace meow
