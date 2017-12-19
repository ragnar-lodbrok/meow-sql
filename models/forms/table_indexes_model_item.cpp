#include "table_indexes_model_item.h"

namespace meow {
namespace models {
namespace forms {

ITableIndexesModelItem::ITableIndexesModelItem(ITableIndexesModelItem * parent)
    : //QObject(parent),
      _parent(parent)
{

}

TableIndexesModelItemIndex::TableIndexesModelItemIndex(
        db::TableIndex * index,
        ITableIndexesModelItem * parent)
    : ITableIndexesModelItem(parent),
      _index(index)
{

}

} // namespace forms
} // namespace models
} // namespace meow
