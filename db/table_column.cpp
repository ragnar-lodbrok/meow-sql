#include "table_column.h"

namespace meow {
namespace db {

TableColumn::TableColumn()
    :_dataType(std::make_shared<DataType>()),
     _unsigned(false),
     _allowNull(false),
     _zeroFill(false),
     _defaultType(ColumnDefaultType::None),
     _id(0)
{

}

TableColumn::operator QString() const
{
    QString str = QString("name:%1 type:%2")
        .arg(_name)
        .arg(this->dataTypeName());

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

bool TableColumn::dataDiffers(const TableColumn * other) const
{
    if (this == other) return false;

    if (_name != other->_name) return true;
    if (_dataType != other->_dataType) return true; // TODO: rm?

    // TODO: check for nullptr
    // TODO: compare types in ConnectionDataType
    //if (_dataType->index != other->_dataType->index) return true;
    if (_lengthSet != other->_lengthSet) return true;
    if (_unsigned != other->_unsigned) return true;
    if (_allowNull != other->_allowNull) return true;
    if (_zeroFill != other->_zeroFill) return true;
    if (_defaultType != other->_defaultType) return true;
    if (_defaultText != other->_defaultText) return true;
    if (_comment != other->_comment) return true;
    if (_charset != other->_charset) return true;
    if (_collation != other->_collation) return true;
    // _id ?
    // TODO: compare strings so "" == QString() ?

    return false;
}

} // namespace db
} // namespace meow
