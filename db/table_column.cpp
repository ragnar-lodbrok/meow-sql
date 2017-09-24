#include "table_column.h"

namespace meow {
namespace db {

TableColumn::TableColumn()
    :_dataType(DataTypeIndex::None),
     _unsigned(false),
     _allowNull(false),
     _zeroFill(false),
     _defaultType(ColumnDefaultType::None)
{

}

} // namespace db
} // namespace meow
