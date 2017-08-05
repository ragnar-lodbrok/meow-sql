#ifndef DB_DATA_TYPE_H
#define DB_DATA_TYPE_H

#include <QString>
#include "data_type_category.h"

namespace meow {
namespace db {

enum class DataTypeIndex {

    None,
    TinyInt,
    SmallInt,
    MediumInt,
    Int,
    BigInt,
    Serial,
    BigSerial,
    Float,
    Double,
    Decimal,
    Numeric,
    Real,
    DoublePrecision,
    Money,
    SmallMoney,
    Date,
    Time,
    Year,
    DateTime,
    DateTime2,
    Smalldatetime,
    Timestamp,
    Interval,
    Char,
    Nchar,
    Varchar,
    Nvarchar,
    TinyText,
    Text,
    NText,
    MediumText,
    LongText,
    Json,
    Cidr,
    Inet,
    Macaddr,
    Binary,
    Varbinary,
    Tinyblob,
    Blob,
    Mediumblob,
    Longblob,
    Image,
    Enum,
    Set,
    Bit,
    VarBit,
    Bool,
    Unknown,
    Cursor,
    Sqlvariant,
    Table,
    Uniqueidentifier,
    Hierarchyid,
    Point,
    Linestring,
    LineSegment,
    Polygon,
    Geometry,
    Box,
    Path,
    Circle,
    Multipoint,
    Multilinestring,
    Multipolygon,
    Geometrycollection
};

DataTypeCategoryIndex categoryOfDataType(DataTypeIndex type);

typedef struct DataType {

    DataType() : index(DataTypeIndex::None), nativeType(-1), name(), names(),
                 description(), hasLength(false), requiresLength(false),
                 hasBinary(false), hasDefault(false), loadPart(false),
                 defLengthSet(), format(),
                 categoryIndex(DataTypeCategoryIndex::None) { }

    DataTypeIndex index;
    int nativeType;
    QString name;
    QString names;
    QString description;
    bool hasLength;
    bool requiresLength;
    bool hasBinary;
    bool hasDefault;
    bool loadPart;
    QString defLengthSet;
    QString format;
    DataTypeCategoryIndex categoryIndex;

} DataType;



} // namespace db
} // namespace meow

#endif // DB_DATA_TYPE_H
