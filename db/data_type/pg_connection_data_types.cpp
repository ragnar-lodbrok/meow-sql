#include "pg_connection_data_types.h"
#include "db/pg/pg_connection.h"
#include "db/query.h"
#include "helpers/logger.h"

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

        // fill manually to 1) run query 2) have fallback if query fails

        fillListManually();
        fillMapFromList();

        try {
            auto list = selectListFromDB();
            _list = list;
            fillMapFromList();
        } catch(meow::db::Exception & ex) {
            meowLogC(Log::Category::Error)
                    << "Failed to select types from db: " << ex.message();
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

void PGConnectionDataTypes::fillMapFromList()
{
    // speed up search
    _map.clear();

    for (DataTypePtr & dataTypeIt : _list) {
        _map.insert(dataTypeIt->nativeType, dataTypeIt);
    }
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

QList<DataTypePtr> PGConnectionDataTypes::selectListFromDB() const
{
    // select ony "user-space" types, hide "system"
    const QString SQL = "SELECT OID, typname, typlen, typcategory FROM pg_type"
                        " WHERE typcategory NOT IN ('A', 'C', 'P', 'R')"
                        " ORDER BY typcategory, typname;";

    QueryPtr queryPtr = _connection->getResults(SQL);
    Query * query = queryPtr.get();

    std::size_t indexOfOid  = query->indexOfColumn("oid");
    std::size_t indexOfName = query->indexOfColumn("typname");
    std::size_t indexOfLength = query->indexOfColumn("typlen");
    std::size_t indexOfCategory = query->indexOfColumn("typcategory");

    QList<DataTypePtr> list;

    meowLogDebug() << "Got " << query->recordCount() << " types from database";

    while (query->isEof() == false) {
        QString oidStr = query->curRowColumn(indexOfOid);
        QString name = query->curRowColumn(indexOfName);
        int length = query->curRowColumn(indexOfLength).toInt();
        QString categoryStr = query->curRowColumn(indexOfCategory);

        Oid oid = static_cast<Oid>(oidStr.toInt());
        bool hasVarLength = length < 0; // see the doc for pg_type
        char categoryChar = categoryStr.length() == 1 ?
                categoryStr[0].toLatin1() : 0;

        list.append(DataTypePtr(new DataType(
            DataTypeIndex::SeeNativeType,
            static_cast<int>(oid),
            name,
            hasVarLength, // hasLength
            category(categoryChar, oid)
        )));

        query->seekNext();
    }

    return list;
}

DataTypeCategoryIndex PGConnectionDataTypes::category(
        const char category, Oid type) const
{
    // TODO: look for typbasetype for domain types

    switch (category) {

    case 'B':
        return DataTypeCategoryIndex::Other; // TODO: add Boolean cathegory?

    case 'D':
        return DataTypeCategoryIndex::Temporal;

    case 'G':
        return DataTypeCategoryIndex::Spatial;

    case 'N':
        if (isIntegerBasicType(type)) {
            return DataTypeCategoryIndex::Integer;
        }
        return DataTypeCategoryIndex::Float;

    case 'S':
        return DataTypeCategoryIndex::Text;

    case 'U':
        if (type == 17) { // bytea, TODO: use const
            return DataTypeCategoryIndex::Binary;
        }
        return DataTypeCategoryIndex::Other;

    // TODO: 'V' ?

    // TODO: support for regprocedure and other reg* types

    default:
        return DataTypeCategoryIndex::Other;

    }
}

bool PGConnectionDataTypes::isIntegerBasicType(Oid type) const
{
    // TODO: is it safer to use name?

    // TODO: use consts for 20, 21 etc
    switch (type) {
    case 20: // int8
    case 21: // int2
    case 23: // int4
    case 26: // oid
        return true;

    default:
        return false;
    }
}

} // namespace db
} // namespace meow

