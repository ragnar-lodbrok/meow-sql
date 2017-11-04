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
    QString str = QString("name:%1 type:%2")
        .arg(_name)
        .arg(dataTypeName(_dataType));

    if (!_lengthSet.isEmpty()) {
        str += QString(" len/set:") + _lengthSet;
    }

    if (_unsigned) {
        str += " UNSIGNED";
    }

    if (_zeroFill) {
        str += " ZEROFILL";
    }

    if (!_charset.isEmpty()) {
        str += " charset:" + _charset;
    }

    if (!_collation.isEmpty()) {
        str += " collate:" + _collation;
    }

    if (_defaultType != ColumnDefaultType::None) {
        str += " default:" + columnDefaultType2String(_defaultType);
        if (_defaultType == ColumnDefaultType::Text
                || _defaultType == ColumnDefaultType::TextUpdateTS) {
            str += " '" + _defaultText + "'";
        }
    }

    if (!_comment.isNull()) {
        str += " comment:" + _comment;
    }

    return str;
}

const QString columnDefaultType2String(ColumnDefaultType type)
{
    switch (type) {
    case ColumnDefaultType::None:
        return "None";

    case ColumnDefaultType::Text:
        return "Text";

    case ColumnDefaultType::TextUpdateTS:
        return "Text ON UPDATE CURRENT_TIMESTAMP";

    case ColumnDefaultType::Null:
        return "NULL";

    case ColumnDefaultType::NullUpdateTS:
        return "NULL ON UPDATE CURRENT_TIMESTAMP";

    case ColumnDefaultType::CurTS:
        return "CURRENT_TIMESTAMP";

    case ColumnDefaultType::CurTSUpdateTS:
        return "CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP";

    case ColumnDefaultType::AutoInc:
        return "AUTO_INCREMENT";

    default:
        return "UNKNOWN";
    }
}

} // namespace db
} // namespace meow
