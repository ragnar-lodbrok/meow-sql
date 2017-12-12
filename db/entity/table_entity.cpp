#include "table_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

TableEntity::TableEntity(const QString & tableName, DataBaseEntity * parent)
    :Entity(parent),
     _tableName(tableName),
     _rowsCount(0),
     _dataSize(0),
     _version(0),
     _structure(nullptr)
{

}

TableEntity::~TableEntity()
{
    delete _structure;
}

QString TableEntity::name() const // override
{
    return _tableName;
}

QVariant TableEntity::icon() const // override
{
    static const QIcon icon = QIcon(":/icons/table.png");

    return icon;
}

db::ulonglong TableEntity::rowsCount(bool refresh /*= false*/)
{
    if (refresh) {
        _rowsCount = connection()->getRowCount(this);
    }
    return _rowsCount;
}

db::TableStructure * TableEntity::structure() const
{
    if (!_structure) {
        _structure = new TableStructure();
    }
    return _structure;
}

} // namespace db
} // namespace meow
