#include "table_indexes_model_item.h"
#include "table_indexes_model.h"
#include "db/table_index.h"

namespace meow {
namespace models {
namespace forms {

ITableIndexesModelItem::ITableIndexesModelItem(ITableIndexesModelItem * parent)
    :
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

TableIndexesModelItemIndex::~TableIndexesModelItemIndex()
{
    qDeleteAll(_columns);
}

QString TableIndexesModelItemIndex::data(int col) const
{
    switch (static_cast<TableIndexesModel::Columns>(col)) {
    case TableIndexesModel::Columns::Name:
        return _index->name();
    default:
        return QString();
    }
}

} // namespace forms
} // namespace models
} // namespace meow
