#ifndef DB_DATA_TYPE_H
#define DB_DATA_TYPE_H


namespace meow {
namespace db {

enum DataTypeIndex {

    None,
    TinyInt,
    Smallint,
    Mediumint,
    Int,
    Bigint,
    Serial,
    BigSerial,
    Float,
    Double,
    Decimal,
    Numeric,
    Real,
    DoublePrecision,
    Money,
    Smallmoney,
    Date,
    Time,
    Year,
    Datetime,
    Datetime2,
    Smalldatetime,
    Timestamp,
    Interval,
    Char,
    Nchar,
    Varchar,
    Nvarchar,
    Tinytext,
    Text,
    Ntext,
    Mediumtext,
    Longtext,
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

enum DataTypeCategoryIndex {
    Integer,
    Real,
    Text,
    Binary,
    Temporal,
    Spatial,
    Other
};

typedef struct DataType {

    DataType() : index(None), nativeType(-1), name(), names(),
                 description(), hasLength(false), requiresLength(false),
                 hasBinary(false), hasDefault(false), loadPart(false),
                 defLengthSet(), format(), categoryIndex(Other) { }

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
