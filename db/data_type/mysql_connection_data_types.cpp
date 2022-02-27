#include "mysql_connection_data_types.h"
#include "db/mysql/mysql_connection.h"

namespace meow {
namespace db {

// To distinguish between binary and nonbinary data for string data types,
// check whether the charsetnr value is 63.
static const int MYSQL_BINARY_CHARSET_NUMBER = 63;
static const int MYSQL_JSON_NATIVE_TYPE_ID = 245;

MySQLConnectionDataTypes::MySQLConnectionDataTypes(MySQLConnection * connection)
    : ConnectionDataTypes(connection),
      _init(false)
{

}

const QList<DataTypePtr> & MySQLConnectionDataTypes::list()
{

    Q_ASSERT(_connection->active());

    // TODO // Support microseconds in some temporal datatypes of MariaDB 5.3+
    // and MySQL 5.6

    if (_init == false) {
        _init = true;

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::TinyInt,
            FIELD_TYPE_TINY,
            "TINYINT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::SmallInt,
            FIELD_TYPE_SHORT,
            "SMALLINT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::MediumInt,
            FIELD_TYPE_INT24,
            "MEDIUMINT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Int,
            FIELD_TYPE_LONG,
            "INT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::BigInt,
            FIELD_TYPE_LONGLONG,
            "BIGINT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Float,
            FIELD_TYPE_FLOAT,
            "FLOAT",
            false, // hasLength
            DataTypeCategoryIndex::Float
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Double,
            FIELD_TYPE_DOUBLE,
            "DOUBLE",
            false, // hasLength
            DataTypeCategoryIndex::Float
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Decimal,
            FIELD_TYPE_NEWDECIMAL,
            "DECIMAL",
            true, // hasLength
            DataTypeCategoryIndex::Float,
            "10,0" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Date,
            FIELD_TYPE_DATE,
            "DATE",
            false, // hasLength
            DataTypeCategoryIndex::Temporal
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Time,
            FIELD_TYPE_TIME,
            "TIME",
            false, // hasLength
            DataTypeCategoryIndex::Temporal
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Year,
            FIELD_TYPE_YEAR,
            "YEAR",
            false, // hasLength
            DataTypeCategoryIndex::Temporal
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::DateTime,
            FIELD_TYPE_DATETIME,
            "DATETIME",
            false, // hasLength
            DataTypeCategoryIndex::Temporal
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Timestamp,
            FIELD_TYPE_TIMESTAMP,
            "TIMESTAMP",
            false, // hasLength
            DataTypeCategoryIndex::Temporal
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Char,
            FIELD_TYPE_STRING,
            "CHAR",
            true, // hasLength
            DataTypeCategoryIndex::Text,
            "50" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Varchar,
            FIELD_TYPE_VAR_STRING,
            "VARCHAR",
            true, // hasLength
            DataTypeCategoryIndex::Text,
            "50" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::TinyText,
            FIELD_TYPE_TINY_BLOB,
            "TINYTEXT",
            false, // hasLength
            DataTypeCategoryIndex::Text
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Text,
            FIELD_TYPE_BLOB,
            "TEXT",
            true, // hasLength
            DataTypeCategoryIndex::Text
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::MediumText,
            FIELD_TYPE_MEDIUM_BLOB,
            "MEDIUMTEXT",
            false, // hasLength
            DataTypeCategoryIndex::Text
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::LongText,
            FIELD_TYPE_LONG_BLOB,
            "LONGTEXT",
            false, // hasLength
            DataTypeCategoryIndex::Text
        )));

        // TODO: JSON is available since MySQL 5.7.8 - check version?
        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Json,
            MYSQL_JSON_NATIVE_TYPE_ID, // native type
            "JSON",
            false, // hasLength
            DataTypeCategoryIndex::Text
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Binary,
            FIELD_TYPE_STRING, // ?
            "BINARY",
            true, // hasLength
            DataTypeCategoryIndex::Binary,
            "50" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Varbinary,
            FIELD_TYPE_VAR_STRING,
            "VARBINARY",
            true, // hasLength
            DataTypeCategoryIndex::Binary,
            "50" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Tinyblob,
            FIELD_TYPE_TINY_BLOB,
            "TINYBLOB",
            false, // hasLength
            DataTypeCategoryIndex::Binary
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Blob,
            FIELD_TYPE_BLOB,
            "BLOB",
            false, // hasLength
            DataTypeCategoryIndex::Binary
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Mediumblob,
            FIELD_TYPE_MEDIUM_BLOB,
            "MEDIUMBLOB",
            false, // hasLength
            DataTypeCategoryIndex::Binary
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Longblob,
            FIELD_TYPE_LONG_BLOB,
            "LONGBLOB",
            false, // hasLength
            DataTypeCategoryIndex::Binary
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Enum,
            FIELD_TYPE_ENUM,
            "ENUM",
            true, // hasLength
            DataTypeCategoryIndex::Other,
            "'Y','N'" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Set,
            FIELD_TYPE_SET,
            "SET",
            true, // hasLength
            DataTypeCategoryIndex::Other,
            "'A','B'" // defLengthSet
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Bit,
            FIELD_TYPE_BIT,
            "BIT",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Geometry,
            FIELD_TYPE_GEOMETRY,
            "GEOMETRY",
            false, // hasLength
            DataTypeCategoryIndex::Spatial
        )));

        // TODO: point/polygon/etc


        // speed up search
        for (DataTypePtr & dataTypeIt : _list) {
            _map.insert(dataTypeIt->index, dataTypeIt);
        }

    }

    return _list;
}

const DataTypePtr MySQLConnectionDataTypes::defaultType() const
{
    // TODO: use string by default?
    return _map.value(DataTypeIndex::Int);
}

bool MySQLConnectionDataTypes::isDateTimeType(const DataTypePtr & type) const
{
    return type->index == DataTypeIndex::DateTime
            || type->index == DataTypeIndex::Timestamp;
}

bool MySQLConnectionDataTypes::isDateType(const DataTypePtr & type) const
{
    return type->index == DataTypeIndex::Date;
}

bool MySQLConnectionDataTypes::isTimeType(const DataTypePtr & type) const
{
    return type->index == DataTypeIndex::Time;
}

bool MySQLConnectionDataTypes::isYearType(const DataTypePtr & type) const
{
    return type->index == DataTypeIndex::Year;
}

bool MySQLConnectionDataTypes::isEnumType(const DataTypePtr & type) const
{
    return type->index == DataTypeIndex::Enum;
}

DataTypePtr MySQLConnectionDataTypes::dataTypeOfField(MYSQL_FIELD * field)
{
    // http://dev.mysql.com/doc/refman/5.7/en/c-api-data-structures.html

    bool isStringType = field->type == FIELD_TYPE_STRING;

    // ENUM and SET values are returned as strings. For these, check that
    // the type value is MYSQL_TYPE_STRING and that the ENUM_FLAG or SET_FLAG
    // flag is set in the flags value.
    if (isStringType) {
        if (field->flags & ENUM_FLAG) {
            return dataTypeFromNative(FIELD_TYPE_ENUM);
        } else if (field->flags & SET_FLAG) {
            return dataTypeFromNative(FIELD_TYPE_SET);
        }
    }

    // To distinguish between binary and nonbinary data for string data types,
    // check whether the charsetnr value is 63. If so, the character set is
    // binary, which indicates binary rather than nonbinary data. This enables
    // you to distinguish BINARY from CHAR, VARBINARY from VARCHAR, and the BLOB
    // types from the TEXT types.
    bool isBinary;
    if (_connection->isUnicode()) {
        isBinary = field->charsetnr == MYSQL_BINARY_CHARSET_NUMBER;
    } else { // TODO: not sure we need this
        isBinary = (field->flags & BINARY_FLAG);
    }

    return dataTypeFromNative(field->type, isBinary);
}

DataTypePtr MySQLConnectionDataTypes::dataTypeFromNative(
        const int nativeDatatype,
        bool isBinary)
{
    list(); // init

    DataTypeIndex dataTypeIndex
            = dataTypeIndexFromNative(nativeDatatype, isBinary);

    DataTypePtr type = _map.value(dataTypeIndex, nullptr);
    if (type) {
        return type;
    }

    return createUnknownType();
}

DataTypeIndex MySQLConnectionDataTypes::dataTypeIndexFromNative(
        const int nativeDatatype,
        bool isBinary)
{
    if (isBinary) {

        switch (nativeDatatype) {
        case FIELD_TYPE_STRING: // char
            return DataTypeIndex::Binary;

        case FIELD_TYPE_VAR_STRING: // varchar
            return DataTypeIndex::Varbinary;

        case FIELD_TYPE_TINY_BLOB:
            return DataTypeIndex::Tinyblob;

        case FIELD_TYPE_BLOB:
            return DataTypeIndex::Blob;

        case FIELD_TYPE_MEDIUM_BLOB:
            return DataTypeIndex::Mediumblob;

        case FIELD_TYPE_LONG_BLOB:
            return DataTypeIndex::Longblob;

        default:
            break;
        }
    }

    switch (nativeDatatype) {
    case FIELD_TYPE_TINY:
        return DataTypeIndex::TinyInt;

    case FIELD_TYPE_SHORT:
        return DataTypeIndex::SmallInt;

    case FIELD_TYPE_INT24:
        return DataTypeIndex::MediumInt;

    case FIELD_TYPE_LONG:
        return DataTypeIndex::Int;

    case FIELD_TYPE_LONGLONG:
        return DataTypeIndex::BigInt;

    case FIELD_TYPE_FLOAT:
        return DataTypeIndex::Float;

    case FIELD_TYPE_DOUBLE:
        return DataTypeIndex::Double;

    case FIELD_TYPE_NEWDECIMAL:
        return DataTypeIndex::Decimal;

    case FIELD_TYPE_DATE:
        return DataTypeIndex::Date;

    case FIELD_TYPE_TIME:
        return DataTypeIndex::Time;

    case FIELD_TYPE_YEAR:
        return DataTypeIndex::Year;

    case FIELD_TYPE_DATETIME:
        return DataTypeIndex::DateTime;

    case FIELD_TYPE_TIMESTAMP:
        return DataTypeIndex::Timestamp;

    case FIELD_TYPE_STRING:
        return DataTypeIndex::Char;

    case FIELD_TYPE_VAR_STRING:
        return DataTypeIndex::Varchar;

    case FIELD_TYPE_TINY_BLOB:
        return DataTypeIndex::TinyText;

    case FIELD_TYPE_BLOB:
        return DataTypeIndex::Text;

    case FIELD_TYPE_MEDIUM_BLOB:
        return DataTypeIndex::MediumText;

    case FIELD_TYPE_LONG_BLOB:
        return DataTypeIndex::LongText;

    case MYSQL_JSON_NATIVE_TYPE_ID:
        return DataTypeIndex::Json;

    case FIELD_TYPE_ENUM:
        return DataTypeIndex::Enum;

    case FIELD_TYPE_SET:
        return DataTypeIndex::Set;

    case FIELD_TYPE_BIT:
        return DataTypeIndex::Bit;

    case FIELD_TYPE_GEOMETRY: // TODO: point/polygon/etc
        return DataTypeIndex::Geometry;


    default:
        return DataTypeIndex::None;
    }
}

} // namespace db
} // namespace meow
