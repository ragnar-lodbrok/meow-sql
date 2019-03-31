#ifndef DB_PG_DATA_TYPE_H
#define DB_PG_DATA_TYPE_H

#include "data_type.h"
#include <postgresql/libpq-fe.h>

// @see src/include/catalog/pg_type.h
// for compat only when "SELECT OID, * FROM pg_type;" fails

namespace meow {
namespace db {

// TODO: rm?
DataTypeIndex dataTypeFromPGDataType(const Oid nativeDataType)
{

    switch (nativeDataType) {

    case 16: // bool
        return DataTypeIndex::Bool;

    case 17: // bytea
        return DataTypeIndex::Blob;

    case 18: // char
        return DataTypeIndex::Char;

    case 19: // name
        return DataTypeIndex::Varchar;

    case 20: // int8
        return DataTypeIndex::BigInt;

    case 21: // int2
        return DataTypeIndex::SmallInt;

    case 22: // int2vector
        return DataTypeIndex::Text;

    case 23: // int4
        return DataTypeIndex::Int;

    case 24: // regproc
        return DataTypeIndex::Varchar;

    case 25: // text
        return DataTypeIndex::Text;

    case 26: // oid
        return DataTypeIndex::Int;

    case 27: // tid
        return DataTypeIndex::None; // TODO?

    case 28: // xid
        return DataTypeIndex::Int;

    case 29: // cid
        return DataTypeIndex::Int;

    case 30: // oidvector
        return DataTypeIndex::Text;

    case 114: // json
        return DataTypeIndex::Json;

    case 142: // xml
        return DataTypeIndex::Xml;

    case 600: // point
        return DataTypeIndex::Point;

    case 601: // lseg
        return DataTypeIndex::LineSegment;

    case 602: // path
        return  DataTypeIndex::Path;

    case 603: // box
        return DataTypeIndex::Box;

    case 604: // polygon
        return DataTypeIndex::Polygon;

    case 628: // line
        return DataTypeIndex::Linestring;

    case 700: // float4
        return DataTypeIndex::Float;

    case 701: // float8
        return DataTypeIndex::Double;

    case 702: // abstime
        return DataTypeIndex::Timestamp;

    // TODO: 703 704 705

    case 718: // circle
        return DataTypeIndex::Circle;

    case 790: // money
        return DataTypeIndex::Money;

    case 829: // macaddr
        return DataTypeIndex::Macaddr;

    case 869: // inet
        return DataTypeIndex::Inet;

    case 650: // cidr
        return DataTypeIndex::Cidr;

    case 1114: // timestamp
        return DataTypeIndex::Timestamp;

    case 1184: // timestampz
        return DataTypeIndex::Timestamp;

    case 1560: // bit
        return DataTypeIndex::Bit;

    case 1562: // varbit
        return DataTypeIndex::VarBit;

    case 1700: // numeric
        return DataTypeIndex::Numeric;

    }

    return DataTypeIndex::None;
}

} // namespace db
} // namespace meow

#endif // DB_PG_DATA_TYPE_H
