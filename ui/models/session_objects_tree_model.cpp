#include "session_objects_tree_model.h"
#include "db/entity/table_entity.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

using Type = meow::db::Entity::Type;

class SessionObjectsTreeModel::TreeItem
{
public:

    meow::db::Entity * const entity = nullptr;
    TreeItem * const parent = nullptr;
    QVector<TreeItem *> children;
    const QString fieldName;
    const Type type = Type::None;
    bool childrenAdded = false;

    explicit TreeItem(meow::db::Entity * _entity = nullptr,
                      TreeItem * _parent = nullptr)
        : entity(_entity)
        , parent(_parent)
        , type(_entity ? _entity->type() : Type::None)
    {

    }

    explicit TreeItem(const QString & _fieldName,
                      TreeItem * _parent = nullptr)
        : parent(_parent)
        , fieldName(_fieldName)
        , type(Type::Field)
        , childrenAdded(true)
    {

    }

    ~TreeItem() {
        qDeleteAll(children);
    }

    TreeItem * appendChild(meow::db::Entity * entity) {
        Q_ASSERT(childrenAdded == false);
        TreeItem * item = new TreeItem(entity, this);
        children.append(item);
        return item;
    }

    int row() const {
        if (parent) {
            return parent->children.indexOf(const_cast<TreeItem*>(this));
        }
        return 0;
    }

    QString databaseName() const
    {
        const TreeItem * item = this;

        if (item->type == Type::Field) {
            item = item->parent;
        }

        const meow::db::Entity * entity = item ? item->entity : nullptr;
        if (entity) {
            return meow::db::databaseName(entity);
        }
        return QString();
    }
};

SessionObjectsTreeModel::SessionObjectsTreeModel()
    : QAbstractItemModel(nullptr)
    , _session(nullptr)
    , _rootItem(new SessionObjectsTreeModel::TreeItem())
{

}

SessionObjectsTreeModel::~SessionObjectsTreeModel()
{
    delete _rootItem;
}

void SessionObjectsTreeModel::setSession(meow::db::SessionEntity * session)
{
    removeData();
    _session = session;
    insertData();
}

Qt::ItemFlags SessionObjectsTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    flags |= Qt::ItemIsUserCheckable;

    return flags;
}

QModelIndex SessionObjectsTreeModel::index(int row,
                                       int column,
                                       const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem *parentItem;

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem * childItem = parentItem->children.value(row, nullptr);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    return QModelIndex();
}

QModelIndex SessionObjectsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parent;

    if (parentItem == _rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int SessionObjectsTreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem * parentItem;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = _rootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->children.count();
}

int SessionObjectsTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant SessionObjectsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeItem * item = static_cast<TreeItem *>(index.internalPointer());

    if (role == Qt::DisplayRole) {
        if (item->type == Type::Field) {
            return item->fieldName;
        }
        const meow::db::Entity * entity = item->entity;
        if (entity) {
            return entity->name();
        }
    } else if (role == Qt::DecorationRole) {
        if (item->type == Type::Field) {
            return QIcon(":/icons/bullet_white.png");
        }
        const meow::db::Entity * entity = item->entity;
        if (entity) {
            return entity->icon();
        }
    }

    return QVariant();
}

bool SessionObjectsTreeModel::hasChildren(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        TreeItem * parentItem = static_cast<TreeItem *>(
                                            parent.internalPointer()
                                        );

        if (parentItem->type == Type::Field) {
            return false;
        }

        if (parentItem->type == meow::db::Entity::Type::Session
            || parentItem->type == meow::db::Entity::Type::Database
            || parentItem->type == meow::db::Entity::Type::Table) {
            return true;
        }
    }

    return QAbstractItemModel::hasChildren(parent);
}

bool SessionObjectsTreeModel::canFetchMore(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return false;

    TreeItem * parentItem = static_cast<TreeItem *>(
                                        parent.internalPointer());
    return parentItem ? !parentItem->childrenAdded : false;
}

void SessionObjectsTreeModel::fetchMore(const QModelIndex &parent)
{
    TreeItem * item = static_cast<TreeItem *>(parent.internalPointer());

    if (item->type == Type::Field) {
        return;
    }

    meow::db::Entity * entity = item ? item->entity : nullptr;
    if (!entity) {
        return;
    }

    if (item->childrenAdded) {
        return;
    }

    if (entity->type() == meow::db::Entity::Type::Database) {
        QList<meow::db::Entity *> children;
        try {
            for (int i = 0; i < entity->childCount(); ++i) {
                meow::db::Entity * entityIt = entity->child(i);
                if (entityIt->type() == meow::db::Entity::Type::Table
                    || entityIt->type() == meow::db::Entity::Type::View
                    || entityIt->type() == meow::db::Entity::Type::Function
                    || entityIt->type() == meow::db::Entity::Type::Procedure) {
                    children << entityIt;
                }
            }
        } catch(meow::db::Exception & ex) {
            emit fetchObjectError(ex.what());
        }

        int newRows = children.count();
        if (newRows) {
            beginInsertRows(parent, 0, newRows - 1);
            for (meow::db::Entity * childEntity : children) {
                item->appendChild(childEntity);
            }
            endInsertRows();
        }

    } else if (entity->type() == meow::db::Entity::Type::Table) {
         meow::db::TableEntity * table
                     = static_cast<meow::db::TableEntity *>(entity);
         try {
            _session->connection()->parseTableStructure(table);
         } catch(meow::db::Exception & ex) {
             emit fetchObjectError(ex.what());
         }

         const QList<meow::db::TableColumn *> & columns
                 = table->structure()->columns();
         int newRows = columns.count();
         if (newRows) {
             beginInsertRows(parent, 0, newRows - 1);
             for (const meow::db::TableColumn * column : columns) {
                 item->children.append(new TreeItem(column->name(), item));
             }
             endInsertRows();
         }
    }

    item->childrenAdded = true;
}

SessionObjectsTreeModel::Object SessionObjectsTreeModel::objectAt(
        const QModelIndex &index)
{
    Object object;

    if (!index.isValid()) {
        return object;
    }

    TreeItem * item = static_cast<TreeItem *>(index.internalPointer());

    object.type = item->type;

    if (item->type == Type::Field) {
        object.fieldName = item->fieldName;
        object.entityName = item->parent->entity->name();
    } else {
        const meow::db::Entity * entity = item->entity;
        if (entity) {
            object.entityName = entity->name();
        }
    }

    object.databaseName = item->databaseName();

    return object;
}

void SessionObjectsTreeModel::reinitItems()
{
    delete _rootItem;
    _rootItem = new SessionObjectsTreeModel::TreeItem();
    if (!_session) {
        return;
    }
    // add session
    TreeItem * sessionItem = _rootItem->appendChild(_session);
    _rootItem->childrenAdded = true;
    // add databases
    for (int i = 0; i < _session->childCount(); ++i) {
        sessionItem->appendChild(_session->child(i));
    }
    sessionItem->childrenAdded = true;

}

void SessionObjectsTreeModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}

void SessionObjectsTreeModel::insertData()
{
    int newRowCount = _session == nullptr ? 0 : 1; // sessions count

    if (newRowCount > 0) {
        beginInsertRows(QModelIndex(), 0, newRowCount-1);
        reinitItems();
        endInsertRows();
    }
}


} // namespace models
} // namespace ui
} // namespace meow
