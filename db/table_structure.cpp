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

} // namespace db
} // namespace meow
