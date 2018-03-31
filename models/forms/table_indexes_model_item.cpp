#include "table_indexes_model_item.h"
#include "table_indexes_model.h"
#include "db/table_index.h"
#include "app.h"

namespace meow {
namespace models {
namespace forms {

ITableIndexesModelItem::ITableIndexesModelItem(ITableIndexesModelItem * parent)
    :
      _parent(parent)
{

}

// -----------------------------------------------------------------------------

TableIndexesModelItemIndex::TableIndexesModelItemIndex(
        db::TableIndex * index,
        ITableIndexesModelItem * parent)
    : ITableIndexesModelItem(parent),
      _index(index)
{
    reinitColumns();
}

TableIndexesModelItemIndex::~TableIndexesModelItemIndex()
{
    qDeleteAll(_columns);
}

QString TableIndexesModelItemIndex::data(int col) const
{
    if (!_index) {
        return QString();
    }

    switch (static_cast<TableIndexesModel::Columns>(col)) {
    case TableIndexesModel::Columns::Name:
        return _index->name();

    case TableIndexesModel::Columns::Type:
        return _index->classTypeStr();

    case TableIndexesModel::Columns::Algorithm:
        if (_index->indexType() == db::TableIndexType::None) {
            return QString();
        }
        return _index->indexTypeStr();
    default:
        return QString();
    }
}

int TableIndexesModelItemIndex::rowOf(ITableIndexesModelItem * child) const
{
    return _columns.indexOf(child);
}

void TableIndexesModelItemIndex::reinitColumns()
{
      qDeleteAll(_columns);
      _columns.clear();
      if (!_index) {
          return;
      }

      int columnsCount = _index->columnsCount();

      for (int i = 0; i < columnsCount; ++i) {
          auto columnItem = new TableIndexesModelItemColumn(_index,
          const_cast<ITableIndexesModelItem *>((ITableIndexesModelItem *)this));
          _columns.push_back(columnItem);
      }
}

void TableIndexesModelItemIndex::removeColumnAt(int index)
{
    if (index >=0 && index < _columns.size()) {
        delete _columns[index];
        _columns.removeAt(index);
    }
}

ITableIndexesModelItem * TableIndexesModelItemIndex::addColumnAt(int index)
{
    if (!_index) {
        return nullptr;
    }
    auto columnItem = new TableIndexesModelItemColumn(
        _index,
        const_cast<ITableIndexesModelItem *>((ITableIndexesModelItem *)this));

    _columns.insert(index, columnItem);

    return columnItem;
}

QVariant TableIndexesModelItemIndex::icon() const
{
    if (!_index) {
        return QVariant();
    }
    auto icons = meow::app()->settings()->icons();
    return icons->iconForTableIndexClass(_index->classType());
}

// -----------------------------------------------------------------------------

TableIndexesModelItemColumn::TableIndexesModelItemColumn(
        db::TableIndex * index,
        ITableIndexesModelItem * parent)
    : ITableIndexesModelItem(parent),
      _index(index)
{

}

QString TableIndexesModelItemColumn::data(int col) const
{
    if (!_index) {
        return QString();
    }

    switch (static_cast<TableIndexesModel::Columns>(col)) {
    case TableIndexesModel::Columns::Name:
        return _index->columns().at(row());
    //case TableIndexesModel::Columns::Type: // TODO
        //return _index->subParts().value(row());
    default:
        return QString();
    }
}

QVariant TableIndexesModelItemColumn::icon() const
{
    if (!_index) {
        return QVariant();
    }
    // TODO: move to Settings::icons
    static const QIcon iconColumn = QIcon(":/icons/bullet_white.png");
    // TODO: rename icon

    return iconColumn;
}

} // namespace forms
} // namespace models
} // namespace meow
