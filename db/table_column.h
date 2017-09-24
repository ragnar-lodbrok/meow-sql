#ifndef DB_TABLE_COLUMN_H
#define DB_TABLE_COLUMN_H

#include <QString>
#include "data_type/data_type.h"

namespace meow {
namespace db {

enum class ColumnDefaultType {
    None, // H: cdtNothing
    Text,
    TextUpdateTS,
    Null,
    NullUpdateTS,
    CurTS,
    CurTSUpdateTS,
    AutoInc
};

class TableColumn
{
public:
    TableColumn();
private:
    QString _name;
    DataTypeIndex _dataType; // TODO: DataType?
    QString _lengthSet; // ?
    bool _unsigned;
    bool _allowNull;
    bool _zeroFill;
    ColumnDefaultType _defaultType;
    QString _defaultText;
    QString _comment;
    QString _charset;
    QString _collation;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_COLUMN_H
