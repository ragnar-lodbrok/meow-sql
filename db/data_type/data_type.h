#ifndef DB_DATA_TYPE_H
#define DB_DATA_TYPE_H

#include <QString>
#include <QMap>
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
    Linestring, // TODO: => LineString
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

bool dataTypeCanBeUnsigned(DataTypeIndex type);
bool dataTypeCanBeZeroFill(DataTypeIndex type);
bool dataTypeHasLength(DataTypeIndex type);
bool dataTypeLoadPartially(DataTypeIndex type);

inline bool dataTypeIsNumeric(DataTypeIndex type) {
    auto category = categoryOfDataType(type);
    return category == DataTypeCategoryIndex::Float
        || category == DataTypeCategoryIndex::Integer;
}

using DataTypeNamesMap = QMap<DataTypeIndex, QString>;
const DataTypeNamesMap & dataTypeNames();

const QString dataTypeName(DataTypeIndex typeIndex);

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
