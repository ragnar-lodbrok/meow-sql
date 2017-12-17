#include "table_structure.h"

namespace meow {
namespace db {

TableStructure::TableStructure()
    : _avgRowLen(0),
      _autoInc(0),
      _maxRows(0),
      _isCheckSum(false)
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
