#include "pg_connection_data_types.h"
#include "db/pg/pg_connection.h"

// https://www.postgresql.org/docs/11/catalog-pg-type.html

namespace meow {
namespace db {

PGConnectionDataTypes::PGConnectionDataTypes(PGConnection * connection)
    : ConnectionDataTypes(connection),
      _init(false)
{

}

const QList<DataTypePtr> & PGConnectionDataTypes::list()
{
    Q_ASSERT(_connection->active());

    if (_init == false) {
        _init = true;

        fillListManually();

        // speed up search
        for (DataTypePtr & dataTypeIt : _list) {
            _map.insert(dataTypeIt->nativeType, dataTypeIt);
        }

    }

    return _list;
}

DataTypePtr PGConnectionDataTypes::dataTypeFromNative(const Oid nativeDatatype)
{
    list(); // init

    if (nativeDatatype == InvalidOid) {
        return createUnknownType();
    }

    DataTypePtr type = _map.value(nativeDatatype, nullptr);
    if (type) {
        return type;
    }

    return createUnknownType();
}

void PGConnectionDataTypes::fillListManually()
{
    _list.clear();

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        16,
        "bool",
        false, // hasLength
        DataTypeCategoryIndex::Other // TODO: add Boolean category
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        17,
        "bytea",
        false, // hasLength
        DataTypeCategoryIndex::Binary
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        18,
        "char",
        true, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        19,
        "name",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        20,
        "int8",
        false, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        21,
        "int2",
        false, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        23,
        "int4",
        false, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    // Technically it's a number but user sees a string => text
    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        24,
        "regproc",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        25,
        "text",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        26,
        "oid",
        false, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        114,
        "json",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        142,
        "xml",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        600,
        "point",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        601,
        "lseg",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        602,
        "path",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        603,
        "box",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        604,
        "polygon",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        628,
        "line",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        650,
        "cidr",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        700,
        "float4",
        true, // hasLength
        DataTypeCategoryIndex::Float
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        701,
        "float8",
        true, // hasLength
        DataTypeCategoryIndex::Float
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        702,
        "abstime",
        false, // hasLength
        DataTypeCategoryIndex::Temporal
    )));

    // TODO: 703 704 705

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        718,
        "circle",
        false, // hasLength
        DataTypeCategoryIndex::Spatial
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        790,
        "money",
        false, // hasLength
        DataTypeCategoryIndex::Float
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        829,
        "macaddr",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        869,
        "inet",
        false, // hasLength
        DataTypeCategoryIndex::Text
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        1114,
        "timestamp",
        false, // hasLength
        DataTypeCategoryIndex::Temporal
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        1560,
        "bit",
        true, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        1562,
        "varbit",
        true, // hasLength
        DataTypeCategoryIndex::Integer
    )));

    _list.append(DataTypePtr(new DataType(
        DataTypeIndex::SeeNativeType,
        1700,
        "numeric",
        true, // hasLength
        DataTypeCategoryIndex::Float
    )));
}

} // namespace db
} // namespace meow

