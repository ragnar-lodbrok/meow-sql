#include "table_indexes_model.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace models {
namespace forms {

TableIndexesModel::TableIndexesModel(QObject * parent)
    : QAbstractItemModel(parent),
      ITableIndexesModelItem(nullptr),
      _table(nullptr)
{

}

TableIndexesModel::~TableIndexesModel()
{
    qDeleteAll(_items);
}

void TableIndexesModel::setTable(meow::db::TableEntity * table)
{
    removeData();
    _table = table;
    reinitItems();
    insertData();
}

Qt::ItemFlags TableIndexesModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex TableIndexesModel::index(
                             int row,
                             int column,
                             const QModelIndex &parentIndex) const
{

    if (!hasIndex(row, column, parentIndex)) {
        return QModelIndex();
    }

    ITableIndexesModelItem * parentItem = nullptr;
    if (!parentIndex.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem = static_cast<ITableIndexesModelItem *>(
                    parentIndex.internalPointer());
    }

    ITableIndexesModelItem * childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return QModelIndex();
    }
}

QModelIndex TableIndexesModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    ITableIndexesModelItem * childItem =
        static_cast<ITableIndexesModelItem *>(index.internalPointer());
    ITableIndexesModelItem * parentItem = childItem->parent();

    if (parentItem == rootItem()) {
        return QModelIndex();
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

QVariant TableIndexesModel::data(const QModelIndex &index, int role) const
{

    if (role == Qt::DisplayRole || role == Qt::DecorationRole) {

        ITableIndexesModelItem * item
             = static_cast<ITableIndexesModelItem *>(index.internalPointer());

        bool isNameCol = static_cast<Columns>(index.column()) == Columns::Name;

        if (role == Qt::DecorationRole && isNameCol) {
            return item->icon();
        } else if (role == Qt::DisplayRole) {
            return item->data(index.column());
        }
    }

    return QVariant();
}

int TableIndexesModel::rowCount(const QModelIndex &parent) const // override
{
    if (parent.column() > 0) {
        return 0;
    }

    ITableIndexesModelItem * parentItem;

    if (!parent.isValid()) {
        parentItem = rootItem();
    } else {
        parentItem =
            static_cast<ITableIndexesModelItem *>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int TableIndexesModel::columnCount(const QModelIndex &parent) const // override
{
    Q_UNUSED(parent);

    return static_cast<int>(Columns::Count);
}

QVariant TableIndexesModel::headerData(int section,
                    Qt::Orientation orientation,
                    int role) const
{

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Name:
            return QString(tr("Name"));

        case Columns::Type:
            return QString(tr("Type / Length"));

        case Columns::Algorithm:
            return QString(tr("Algorithm"));

        default:
            break;
        }
    }

    return QVariant();

}

int TableIndexesModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Name:
        return 250;
    case Columns::Type:
        return 120;
    case Columns::Algorithm:
        return 100;
    default:
        return 80;
    }
}

void TableIndexesModel::reinitItems()
{
      qDeleteAll(_items);
      _items.clear();
      if (!_table) {
          return;
      }

      QList<meow::db::TableIndex *> & indicies = _table->structure()->indicies();

      for (auto & index : indicies) {
          auto indexItem = new TableIndexesModelItemIndex(index, rootItem());
          _items.push_back(indexItem);
      }

}


ITableIndexesModelItem * TableIndexesModel::child(int row) const
{
    return _items.value(row);
}

int TableIndexesModel::rowOf(ITableIndexesModelItem * child) const
{
    return _items.indexOf(child);
}

ITableIndexesModelItem * TableIndexesModel::rootItem() const
{
    return const_cast<ITableIndexesModelItem *>((ITableIndexesModelItem *)this);
}

void TableIndexesModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}
void TableIndexesModel::insertData()
{
    if (rowCount() > 0) {
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

} // namespace forms
} // namespace models
} // namespace meow
