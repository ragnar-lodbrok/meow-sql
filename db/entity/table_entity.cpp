#include "table_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

TableEntity::TableEntity(const QString & tableName, DataBaseEntity * parent)
    :Entity(parent),
     _tableName(tableName),
     _dataSize(0)
{

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

} // namespace db
} // namespace meow
