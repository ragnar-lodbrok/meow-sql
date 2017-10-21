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

TableColumn::operator QString() const
{
    QString str = QString("name: %1 type: %2")
        .arg(_name)
        .arg(dataTypeName(_dataType));

    if (!_lengthSet.isEmpty()) {
        str += QString(" len/set: ") + _lengthSet;
    }

    if (_unsigned) {
        str += " UNSIGNED";
    }

    if (_zeroFill) {
        str += " ZEROFILL";
    }

    return str;
}

} // namespace db
} // namespace meow
