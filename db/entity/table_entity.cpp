#include "table_entity.h"
#include "database_entity.h"

namespace meow {
namespace db {

TableEntity::TableEntity(const QString & tableName, DataBaseEntity * parent)
    :Entity(parent),
     _tableName(tableName)
{

}

QString TableEntity::name() const // override
{
    return _tableName;
}

} // namespace db
} // namespace meow
