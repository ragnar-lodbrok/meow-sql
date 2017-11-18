#include "table_structure.h"

namespace meow {
namespace db {

TableStructure::TableStructure()
{
}

TableStructure::~TableStructure()
{
    qDeleteAll(_columns);
    qDeleteAll(_indicies);
    qDeleteAll(_foreignKeys);
}

bool TableStructure::hasIndexForColumn(
    const QString & columnName, TableIndexClass indexClass)
{
    for (auto & index : _indicies) {
        if (index->classType() == indexClass
            && index->columns().contains(columnName)) {
            return true;
        }
    }
    return false;
}

} // namespace db
} // namespace meow
