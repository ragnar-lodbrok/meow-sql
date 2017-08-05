#include "data_type.h"

meow::db::DataTypeCategoryIndex meow::db::categoryOfDataType(meow::db::DataTypeIndex type)
{
    switch (type) {
    case DataTypeIndex::TinyInt:
    case DataTypeIndex::SmallInt:
    case DataTypeIndex::MediumInt:
    case DataTypeIndex::Int:
    case DataTypeIndex::BigInt:
    case DataTypeIndex::Serial:
    case DataTypeIndex::BigSerial:
    case DataTypeIndex::Bit:
    case DataTypeIndex::VarBit:
        return DataTypeCategoryIndex::Integer;
    case DataTypeIndex::Float:
    case DataTypeIndex::Double:
    case DataTypeIndex::Decimal:
    case DataTypeIndex::Numeric:
    case DataTypeIndex::Real:
    case DataTypeIndex::DoublePrecision:
    case DataTypeIndex::Money:
    case DataTypeIndex::SmallMoney:
        return DataTypeCategoryIndex::Float;
    case DataTypeIndex::Date:
    case DataTypeIndex::Time:
    case DataTypeIndex::Year:
    case DataTypeIndex::DateTime:
    case DataTypeIndex::DateTime2:
    case DataTypeIndex::Smalldatetime:
    case DataTypeIndex::Timestamp:
    case DataTypeIndex::Interval:
        return DataTypeCategoryIndex::Temporal;
    case DataTypeIndex::Char:
    case DataTypeIndex::Nchar:
    case DataTypeIndex::Varchar:
    case DataTypeIndex::Nvarchar:
    case DataTypeIndex::TinyText:
    case DataTypeIndex::Text:
    case DataTypeIndex::NText:
    case DataTypeIndex::MediumText:
    case DataTypeIndex::LongText:
    case DataTypeIndex::Json:
    case DataTypeIndex::Cidr:
    case DataTypeIndex::Inet:
    case DataTypeIndex::Macaddr:
    case DataTypeIndex::Uniqueidentifier:
        return DataTypeCategoryIndex::Text;
    case DataTypeIndex::Binary:
    case DataTypeIndex::Varbinary:
    case DataTypeIndex::Tinyblob:
    case DataTypeIndex::Blob:
    case DataTypeIndex::Mediumblob:
    case DataTypeIndex::Longblob:
    case DataTypeIndex::Image:
        return DataTypeCategoryIndex::Binary;
    case DataTypeIndex::Enum:
    case DataTypeIndex::Set:
    case DataTypeIndex::Bool:
    case DataTypeIndex::Unknown:
    case DataTypeIndex::Cursor:
    case DataTypeIndex::Sqlvariant:
    case DataTypeIndex::Table:
    case DataTypeIndex::Hierarchyid:
        return DataTypeCategoryIndex::Other;
    case DataTypeIndex::Point:
    case DataTypeIndex::Linestring:
    case DataTypeIndex::LineSegment:
    case DataTypeIndex::Polygon:
    case DataTypeIndex::Geometry:
    case DataTypeIndex::Box:
    case DataTypeIndex::Path:
    case DataTypeIndex::Circle:
    case DataTypeIndex::Multipoint:
    case DataTypeIndex::Multilinestring:
    case DataTypeIndex::Multipolygon:
    case DataTypeIndex::Geometrycollection:
        return DataTypeCategoryIndex::Spatial;

    case DataTypeIndex::None:
    default:
        return DataTypeCategoryIndex::None;
    }
}
