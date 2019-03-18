#ifndef DB_DATA_TYPE_H
#define DB_DATA_TYPE_H

#include <QString>
#include <QMap>
#include <memory>
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
    Xml,
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
    Geometrycollection,
    SeeNativeType
};

DataTypeCategoryIndex categoryOfDataType(DataTypeIndex type); // TODO: rm

bool dataTypeCanBeUnsigned(DataTypeIndex type);
bool dataTypeCanBeZeroFill(DataTypeIndex type);
bool dataTypeHasLength(DataTypeIndex type); // TODO: rm
bool dataTypeLoadPartially(DataTypeIndex type);

inline bool dataTypeIsNumeric(DataTypeIndex type) { // TODO: rm
    auto category = categoryOfDataType(type);
    return category == DataTypeCategoryIndex::Float
        || category == DataTypeCategoryIndex::Integer;
}

using DataTypeNamesMap = QMap<DataTypeIndex, QString>; // TODO: rm
const DataTypeNamesMap & dataTypeNames(); // TODO: rm

const QString dataTypeName(DataTypeIndex typeIndex); // TODO: rm

typedef struct DataType {

    DataType() : index(DataTypeIndex::None),
                 nativeType(-1),
                 name(),
                 //description(),
                 hasLength(false),
                 requiresLength(false),
                 hasBinary(false),
                 isBinary(false),
                 //hasDefault(false),
                 //loadPart(false),
                 //defLengthSet(),
                 //format(),
                 categoryIndex(DataTypeCategoryIndex::None) { }

    DataType(
            DataTypeIndex index_,
            int nativeType_,
            const QString & name_,
            bool hasLength_,
            DataTypeCategoryIndex _category
    ) : index(index_),
        nativeType(nativeType_),
        name(name_),
        //description(),
        hasLength(hasLength_),
        requiresLength(false),
        hasBinary(false),
        isBinary(false),
        //hasDefault(false),
        //loadPart(false),
        //defLengthSet(),
        //format(),
        categoryIndex(_category) { }

    DataTypeIndex index;
    int nativeType;
    QString name;
    //QString description;
    bool hasLength;
    bool requiresLength;
    bool hasBinary;
    bool isBinary;
    //bool hasDefault;
    //bool loadPart;
    //QString defLengthSet;
    //QString format;
    DataTypeCategoryIndex categoryIndex;

} DataType;


using DataTypePtr = std::shared_ptr<DataType>;
using DataTypePtrConst = std::shared_ptr<const DataType>;

} // namespace db
} // namespace meow

#endif // DB_DATA_TYPE_H
