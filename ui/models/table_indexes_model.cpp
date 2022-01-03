#include "table_indexes_model.h"
#include "db/entity/table_entity.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

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
        return Qt::NoItemFlags;
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (isEditingSupported() && isEditingAllowed(index)) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
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

    if (role == Qt::DisplayRole ||
        role == Qt::DecorationRole ||
        role == Qt::EditRole) {

        ITableIndexesModelItem * item
             = static_cast<ITableIndexesModelItem *>(index.internalPointer());

        bool isNameCol = static_cast<Columns>(index.column()) == Columns::Name;

        if (role == Qt::DecorationRole && isNameCol) {
            return item->icon();
        } else if (role == Qt::DisplayRole || role == Qt::EditRole) {
            return item->data(index.column());
        }
    }

    return QVariant();
}

bool TableIndexesModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    if (editData(index, value)) {
        emit dataChanged(index, index);
        emit modified();
        return true;
    }

    return false;
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

bool TableIndexesModel::isEditingSupported() const
{
    if (_table == nullptr) return false;
    return _table->connection()->features()->supportsEditingTablesStructure();
}

bool TableIndexesModel::editData(const QModelIndex & index,
                                 const QVariant & value)
{
    ITableIndexesModelItem * item
        = static_cast<ITableIndexesModelItem *>(index.internalPointer());
    if (item == nullptr) return false;

    db::TableIndex * tableIndex = item->tableIndex();

    if (item->type() == ITableIndexesModelItem::Type::Index) {

        switch (static_cast<Columns>(index.column())) {

        case Columns::Name: {
            tableIndex->setName( value.toString() );
            return true;
        }

        case Columns::Type: {
            tableIndex->setClassType( value.toString() );
            return true;
        }

        case Columns::Algorithm: {
            tableIndex->setIndexType( value.toString() );
            return true;
        }

        default:
            break;
        }

    } else if (item->type() == ITableIndexesModelItem::Type::Column) {
        switch (static_cast<Columns>(index.column())) {

        case Columns::Name: {
            if (tableIndex->replaceColumn(index.row(), value.toString())) {
                return true;
            }
        }

        default:
            break;
        }
    }

    return false;
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

bool TableIndexesModel::canAddIndex() const
{
    return isEditingSupported();
}

int TableIndexesModel::insertEmptyDefaultIndex()
{
    int insertIndex = rowCount();

    beginInsertRows(QModelIndex(), insertIndex, insertIndex);

    auto newIndex = _table->structure()->insertEmptyDefaultIndex();
    auto indexItem = new TableIndexesModelItemIndex(newIndex, rootItem());
    _items.push_back(indexItem);

    endInsertRows();

    emit modified();

    return insertIndex;
}

bool TableIndexesModel::canAddColumn(const QModelIndex & curIndex) const
{
    if (_table == nullptr || curIndex.isValid() == false) return false;
    return isEditingSupported(); // anytime we have a selection
    // TODO: check if we have "free" columns?
}

QModelIndex TableIndexesModel::insertEmptyColumn(const QModelIndex & curIndex)
{
    if (!canAddColumn(curIndex)) return {};
    ITableIndexesModelItem * item
        = static_cast<ITableIndexesModelItem *>(curIndex.internalPointer());
    if (item == nullptr) return {};

    int indexRow;
    TableIndexesModelItemIndex * indexItem;
    QModelIndex indexModelIndex;

    if (item->type() == ITableIndexesModelItem::Type::Index) {
        indexRow = item->row();
        indexItem = static_cast<TableIndexesModelItemIndex *>(item);
        indexModelIndex = curIndex;
    } else if (item->type() == ITableIndexesModelItem::Type::Column) {
        indexRow = item->parent()->row();
        indexItem = static_cast<TableIndexesModelItemIndex *>(item->parent());
        indexModelIndex = curIndex.parent();
    } else {
        return {};
    }

    int newColumnIndex = _table->structure()
                               ->insertEmptyDefaultColumnToIndex(indexRow);

    if (newColumnIndex != -1) {

        beginInsertRows(indexModelIndex, newColumnIndex, newColumnIndex);
        auto modelItem = indexItem->addColumnAt(newColumnIndex);
        endInsertRows();

        emit modified();

        return createIndex( // TODO: pbly wrong
            newColumnIndex,
            (int)models::TableIndexesModel::Columns::Name,
            modelItem);
    }

    return {};
}

bool TableIndexesModel::canRemove(const QModelIndex & curIndex) const
{
    if (_table == nullptr || curIndex.isValid() == false) return false;

    if (!isEditingSupported()) return false;

    ITableIndexesModelItem * item
        = static_cast<ITableIndexesModelItem *>(curIndex.internalPointer());
    if (item == nullptr) return false;
    if (item->type() == ITableIndexesModelItem::Type::Index) {
        int indexRow = item->row();
        return _table->structure()->canRemoveIndex(indexRow);
    } else if (item->type() == ITableIndexesModelItem::Type::Column) {
        int indexRow = item->parent()->row();
        int columnRow = item->row();
        return _table->structure()->canRemoveIndexColumn(indexRow, columnRow);
    }
    return false;
}

bool TableIndexesModel::remove(const QModelIndex & curIndex)
{
    if (!canRemove(curIndex)) return false;

    auto item
        = static_cast<ITableIndexesModelItem *>(curIndex.internalPointer());
    if (item == nullptr) return false;
    if (item->type() == ITableIndexesModelItem::Type::Index) {
        int indexRow = item->row();
        if (_table->structure()->removeIndexAt(indexRow)) {
            beginRemoveRows(QModelIndex(), indexRow, indexRow);
            delete _items[indexRow];
            _items.removeAt(indexRow);
            endRemoveRows();
            emit modified();
            return true;
        }
    } else if (item->type() == ITableIndexesModelItem::Type::Column) {
        int indexRow = item->parent()->row();
        int columnRow = item->row();
        QModelIndex parentIndex = curIndex.parent();
        if (_table->structure()->removeIndexColumn(indexRow, columnRow)) {
            beginRemoveRows(parentIndex, columnRow, columnRow);
            auto parentItem = static_cast<TableIndexesModelItemIndex *>(
                parentIndex.internalPointer());
            parentItem->removeColumnAt(columnRow);
            endRemoveRows();
            emit modified();
            return true;
        }
    }
    return false;
}

bool TableIndexesModel::removeAllIndexColumnsByName(const QString & columnName)
{
    int i = 0;
    QList<QModelIndex> idxToRemove;

    for (auto & index : _table->structure()->indicies()) {
        if (index->hasColumn(columnName)) {
            QModelIndex indexModelIndex = this->index(
                i, 0
            );
            QModelIndex columnModelIndex = this->index(
                index->columnIndex(columnName), 0, indexModelIndex
            );
            idxToRemove.append(columnModelIndex);
        }
        ++i;
    }

    for (const auto & index : idxToRemove) {
        remove(index);
    }

    return true;
}

bool TableIndexesModel::isRowTypeIndex(const QModelIndex & index) const
{
    if (!index.isValid()) return false;
    ITableIndexesModelItem * item
         = static_cast<ITableIndexesModelItem *>(index.internalPointer());

    if (!item) return false;

    return (item->type() == ITableIndexesModelItem::Type::Index);
}

bool TableIndexesModel::isRowTypeColumn(const QModelIndex & index) const
{
    if (!index.isValid()) return false;
    ITableIndexesModelItem * item
         = static_cast<ITableIndexesModelItem *>(index.internalPointer());

    if (!item) return false;

    return (item->type() == ITableIndexesModelItem::Type::Column);
}

const QStringList TableIndexesModel::indexTypes() const
{
    return db::tableIndexClassNames();
}

const QStringList TableIndexesModel::indexAlgorithms() const
{
    return db::tableIndexTypeNames();
}

const QStringList TableIndexesModel::indexColumns() const
{
    if (_table) {
        QStringList columns;
        for (const auto & column : _table->structure()->columns()) {
            columns << column->name();
        }
        return columns;
    }
    return {};
}

bool TableIndexesModel::canRemoveAll() const
{
    if (!isEditingSupported()) return false;
    return _table->structure()->canRemoveAllIndices();
}

void TableIndexesModel::removeAll()
{
    if (!canRemoveAll()) return;

    _table->structure()->removeAllIndicies();
    removeData();
    reinitItems();

    emit modified();
}

bool TableIndexesModel::canMoveUp(const QModelIndex & curIndex) const
{
    if (_table == nullptr || curIndex.isValid() == false) return false;

    if (!isEditingSupported()) return false;

    ITableIndexesModelItem * item
        = static_cast<ITableIndexesModelItem *>(curIndex.internalPointer());
    if (item == nullptr) return false;
    if (item->type() == ITableIndexesModelItem::Type::Column) {
        int indexRow = item->parent()->row();
        int columnRow = item->row();
        return _table->structure()->canMoveIndexColumnUp(indexRow, columnRow);
    }
    return false;
}

bool TableIndexesModel::canMoveDown(const QModelIndex & curIndex) const
{
    if (_table == nullptr || curIndex.isValid() == false) return false;

    if (!isEditingSupported()) return false;

    ITableIndexesModelItem * item
        = static_cast<ITableIndexesModelItem *>(curIndex.internalPointer());
    if (item == nullptr) return false;
    if (item->type() == ITableIndexesModelItem::Type::Column) {
        int indexRow = item->parent()->row();
        int columnRow = item->row();
        return _table->structure()->canMoveIndexColumnDown(indexRow, columnRow);
    }
    return false;
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

bool TableIndexesModel::isEditingAllowed(const QModelIndex &index) const
{
    ITableIndexesModelItem * item
         = static_cast<ITableIndexesModelItem *>(index.internalPointer());

    if (!item) return false;

    db::TableIndex * tableIndex = item->tableIndex();

    if (item->type() == ITableIndexesModelItem::Type::Index) {
        switch (static_cast<Columns>(index.column())) {
        case Columns::Name:
            return tableIndex->classType() != db::TableIndexClass::PrimaryKey;
        case Columns::Type:
        case Columns::Algorithm:
            return true;
        default:
            break;
        }
    } else if (item->type() == ITableIndexesModelItem::Type::Column) {
        switch (static_cast<Columns>(index.column())) {
        case Columns::Name:
            return true;
        case Columns::Type:
        case Columns::Algorithm:
            return false;
        default:
            break;
        }
    }

    return false;
}

} // namespace models
} // namespace ui
} // namespace meow
