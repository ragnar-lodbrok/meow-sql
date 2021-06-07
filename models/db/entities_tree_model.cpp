#include "helpers/logger.h"
#include "entities_tree_model.h"
#include "db/entity/session_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/table_entity.h"
#include <QDebug>

namespace meow {
namespace models {
namespace db {

class EntitiesTreeModel::TreeItem
{
public:
    // item data
    meow::db::EntityPtr entity = nullptr;
    bool childrenAdded = false;
    // item data

    TreeItem * parent = nullptr;
    QVector<TreeItem *> children;

    explicit TreeItem(const meow::db::EntityPtr & _entity = nullptr,
                      TreeItem * _parent = nullptr)
        : entity(_entity)
        , parent(_parent)
    {

    }

    ~TreeItem() {
        qDeleteAll(children);
    }

    TreeItem * appendChild(const meow::db::EntityPtr & entity) {
        TreeItem * item = new TreeItem(entity, this);
        children.append(item);
        return item;
    }

    void removeChild(TreeItem * item) {
        children.removeOne(item);
        delete item;
    }

    int row() const {
        if (parent) {
            return parent->children.indexOf(const_cast<TreeItem*>(this));
        }
        return 0;
    }
};


EntitiesTreeModel::EntitiesTreeModel(
        meow::db::ConnectionsManager * dbConnectionsManager,
        QObject * parent)
    : QAbstractItemModel(parent)
    , _dbConnectionsManager(dbConnectionsManager)
    , _rootItem(new EntitiesTreeModel::TreeItem(
                    _dbConnectionsManager->retain()))
{

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::connectionOpened,
            this,
            &meow::models::db::EntitiesTreeModel::onConnectionOpened);

    connect(_dbConnectionsManager,
            &meow::db::ConnectionsManager::beforeConnectionClosed,
            this,
            &meow::models::db::EntitiesTreeModel::onConnectionClosed);

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
            this,
            &meow::models::db::EntitiesTreeModel::onDatabasesDataChanged);
}

Qt::ItemFlags EntitiesTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex EntitiesTreeModel::index(int row,
                             int column,
                             const QModelIndex &parent) const
{

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem * parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    TreeItem * childItem = parentItem->children.value(row, nullptr);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex EntitiesTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem * childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem * parentItem = childItem->parent;

    if (parentItem == rootItem()) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int EntitiesTreeModel::rowCount(const QModelIndex &parent) const // override
{
    TreeItem * parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    }

    if (!parentItem) return 0;

    return parentItem->children.count();
}

int EntitiesTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return 1; // TODO: add size column later
}

bool EntitiesTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        TreeItem * parentItem = static_cast<TreeItem *>(
                                            parent.internalPointer());

        meow::db::EntityPtr entity = parentItem->entity;

        if (entity && (entity->type() == meow::db::Entity::Type::Session
            || entity->type() == meow::db::Entity::Type::Database)) {
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

        TreeItem * item = static_cast<TreeItem *>(index.internalPointer());
        if (!item || !item->entity) return QVariant();

        const meow::db::EntityPtr & entity = item->entity;

        if (role == Qt::DecorationRole) {
            return entity->icon();
        } else if (role == Qt::DisplayRole) {
            return entity->name();
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

    TreeItem * item = static_cast<TreeItem *>(index.internalPointer());

    if (!item || !item->entity) {
        meowLogDebug() << "Tree: empty entity selected";
        return;
    }

    const meow::db::EntityPtr & selectedEntity = item->entity;

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
    TreeItem * item = itemForEntity(entity);

    if (item) {
        return createIndex(item->row(), 0, item);
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

void EntitiesTreeModel::onConnectionOpened(meow::db::SessionEntity * session)
{
    Q_ASSERT(rootItem() != nullptr);

    TreeItem * parent = rootItem();
    int newRowIndex = parent->children.size();

    beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    parent->appendChild(session->retain());
    endInsertRows();
}

void EntitiesTreeModel::onConnectionClosed(meow::db::SessionEntity * session)
{
    Q_ASSERT(rootItem() != nullptr);

    TreeItem * parent = rootItem();
    TreeItem * sessionItem = nullptr;

    for (TreeItem * item : parent->children) {
        if (item->entity.get() == session) {
            sessionItem = item;
            break;
        }
    }
    if (sessionItem) {
        beginRemoveRows(QModelIndex(), sessionItem->row(), sessionItem->row());
        parent->removeChild(sessionItem);
        endRemoveRows();
    }

}

void EntitiesTreeModel::onEntityEdited(meow::db::Entity * entity)
{    
    QModelIndex entityIndex = indexForEntity(entity);
    if (entityIndex.isValid()) {
        emit dataChanged(entityIndex, entityIndex);
    }
}

void EntitiesTreeModel::onEntityInserted(meow::db::Entity * entity)
{
    TreeItem * parent = itemForEntity(entity->parent());

    int newRowIndex = parent->children.size();

    beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
    parent->appendChild(entity->retain());
    endInsertRows();
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
    _dbConnectionsManager->createNewEntity(type);
}

void EntitiesTreeModel::refreshActiveSession()
{
    // TODO: use dataChanged and invalidate only active session
    beginResetModel();
    _dbConnectionsManager->refreshActiveSession();
    reinitItems();
    endResetModel();
}

void EntitiesTreeModel::reloadData()
{
    removeData();
    insertData();
}

void EntitiesTreeModel::dropCurrentItem()
{
    meow::db::Entity * curEntity = currentEntity();
    if (!curEntity) return;

    dropEntity(curEntity);
}

void EntitiesTreeModel::dropEntity(meow::db::Entity * entity)
{
    TreeItem * item = itemForEntity(entity);
    if (!item) return;

    TreeItem * parentItem = item->parent;
    if (!parentItem) return;

    QModelIndex index = createIndex(item->row(), 0, item);

    if (_dbConnectionsManager->dropEntity(entity)) {
        beginRemoveRows(parent(index), index.row(), index.row());
        _dbConnectionsManager->activeSession()->removeEntity(entity);
        parentItem->removeChild(item);
        endRemoveRows();
    }
}

EntitiesTreeModel::TreeItem *
EntitiesTreeModel::itemForEntity(meow::db::Entity * entity)
{
    // TODO: rewrite as recursive?

    if (entity == _dbConnectionsManager) {
        return rootItem();
    }

    // session
    meow::db::Entity * session =
            meow::db::findParentEntityOfType(entity,
                meow::db::Entity::Type::Session);
    if (!session) return nullptr;

    TreeItem * sessionItem = nullptr;

    for (TreeItem * itemIt : rootItem()->children) {
        if (itemIt->entity.get() == session) {
            sessionItem = itemIt;
            break;
        }
    }

    if (entity->type() == meow::db::Entity::Type::Session) {
        return sessionItem;
    }

    // database
    meow::db::Entity * database =
                meow::db::findParentEntityOfType(entity,
                    meow::db::Entity::Type::Database);

    if (!database) return nullptr;

    TreeItem * databaseItem = nullptr;

    for (TreeItem * itemIt : sessionItem->children) {
        if (itemIt->entity.get() == database) {
            databaseItem = itemIt;
            break;
        }
    }

    if (entity->type() == meow::db::Entity::Type::Database) {
        return databaseItem;
    }

    // entity in db
    for (TreeItem * itemIt : databaseItem->children) {
        if (itemIt->entity.get() == entity) {
            return itemIt;
        }
    }

    return nullptr;
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
    for (TreeItem * sessionItem : rootItem()->children) {
        for (TreeItem * databaseItem : sessionItem->children) {
            QModelIndex dbEntityIndex = createIndex(
                        databaseItem->row(), 0, databaseItem);
            emit dataChanged(dbEntityIndex, dbEntityIndex);
            // TODO: seems ineffective (can emit range?)
        }
    }
}

bool EntitiesTreeModel::canFetchMore(const QModelIndex & parent) const
{
    if (!parent.isValid())
        return false;

    TreeItem * parentItem = static_cast<TreeItem *>(
                                        parent.internalPointer());
    return parentItem ? (!parentItem->childrenAdded) : false;
}

void EntitiesTreeModel::fetchMore(const QModelIndex & parent)
{
    TreeItem * item = static_cast<TreeItem *>(parent.internalPointer());

    if (item->childrenAdded) {
        return;
    }

    const meow::db::EntityPtr & entity = item->entity;
    if (!entity) return;

    if (entity->type() == meow::db::Entity::Type::Session
        || entity->type() == meow::db::Entity::Type::Database) {
        int childCount = 0;
        try {
            childCount = entity->childCount(); // loads data
        } catch(meow::db::Exception & ex) {
            emit loadDataError(ex.what());
            return;
        }
        beginInsertRows(parent, 0, childCount - 1);
        for (int i = 0; i < entity->childCount(); ++i) {
            meow::db::Entity * childEntity = entity->child(i);
            item->appendChild(childEntity->retain());
        }
        endInsertRows();
    }

    item->childrenAdded = true;
}


void EntitiesTreeModel::reinitItems()
{
    // Listening: ODC - I Love The way You Fall
    _rootItem.reset(new EntitiesTreeModel::TreeItem(
                _dbConnectionsManager->retain()));

    for (const meow::db::SessionEntityPtr & session
                : _dbConnectionsManager->sessions()) {
        _rootItem->appendChild(session);
    }
    _rootItem->childrenAdded = true;
}

void EntitiesTreeModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        _rootItem.reset();
        endRemoveRows();
    }
}

void EntitiesTreeModel::insertData()
{
    int newRowCount = _dbConnectionsManager->childCount(); // sessions count

    if (newRowCount > 0) {
        beginInsertRows(QModelIndex(), 0, newRowCount-1);
        reinitItems();
        endInsertRows();
    }
}


} // namespace db
} // namespace models
} // namespace meow
