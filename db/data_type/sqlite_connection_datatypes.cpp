#include "sqlite_connection_datatypes.h"
#include "db/sqlite/sqlite_connection.h"

namespace meow {
namespace db {

SQLiteConnectionDataTypes::SQLiteConnectionDataTypes(SQLiteConnection *connection)
    : ConnectionDataTypes(connection),
      _init(false)
{

}

const QList<DataTypePtr> & SQLiteConnectionDataTypes::list()
{
    if (_init == false) {
        _init = true;

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Text,
            static_cast<int>(SQLiteTypeAffinity::Text), // native type
            "TEXT",
            true, // hasLength
            DataTypeCategoryIndex::Text
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Numeric,
            static_cast<int>(SQLiteTypeAffinity::Numeric),
            "NUMERIC",
            true, // hasLength
            DataTypeCategoryIndex::Float // TODO: Text?
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Int,
            static_cast<int>(SQLiteTypeAffinity::Integer),
            "INTEGER",
            true, // hasLength
            DataTypeCategoryIndex::Integer
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Real,
            static_cast<int>(SQLiteTypeAffinity::Real),
            "REAL",
            false, // hasLength
            DataTypeCategoryIndex::Float // TODO: Text?
        )));

        _list.append(DataTypePtr(new DataType(
            DataTypeIndex::Blob,
            static_cast<int>(SQLiteTypeAffinity::Blob),
            "BLOB",
            false, // hasLength
            DataTypeCategoryIndex::Binary
        )));

        // speed up search
        for (const DataTypePtr & dataTypeIt : _list) {
            _map.insert(static_cast<SQLiteTypeAffinity>(dataTypeIt->nativeType),
                        dataTypeIt);
        }

    }

    return _list;
}

const DataTypePtr SQLiteConnectionDataTypes::defaultType() const
{
    return _map.value(SQLiteTypeAffinity::Text);
}

DataTypePtr SQLiteConnectionDataTypes::dataTypeFromField(
    const QSqlField & field)
{
    // https://doc.qt.io/qt-5/sql-types.html#qsqlite-sqlite-version-3-data-types
    // https://doc.qt.io/qt-5/qmetatype.html#Type-enum

    list(); // init

    SQLiteTypeAffinity type = SQLiteTypeAffinity::Text;

    switch (static_cast<QMetaType::Type>(field.type())) {

    case QMetaType::Int:
        type = SQLiteTypeAffinity::Integer;
        break;

    case QMetaType::QByteArray:
        type = SQLiteTypeAffinity::Blob;
        break;

    case QMetaType::Double:
        type = SQLiteTypeAffinity::Real;
        break;

    default:
        break;

    }

    return _map.value(type);
}

SQLiteTypeAffinity SQLiteConnectionDataTypes::affinityByName(
        const QString & name)
{

    // https://www.sqlite.org/datatype3.html#determination_of_column_affinity

    QString upperName = name.toUpper();

    // keep order of IFs!

    if (upperName.contains("INT")) {
        return SQLiteTypeAffinity::Integer;
    }

    if (upperName.contains("CHAR")
            || upperName.contains("TEXT")
            || upperName.contains("CLOB")) {
        return SQLiteTypeAffinity::Text;
    }

    if (upperName.contains("BLOB") || upperName.isEmpty()) {
        return SQLiteTypeAffinity::Blob;
    }

    if (upperName.contains("REAL")
            || upperName.contains("FLOA")
            || upperName.contains("DOUB")) {
        return SQLiteTypeAffinity::Real;
    }

    return SQLiteTypeAffinity::Numeric;
}

DataTypePtr SQLiteConnectionDataTypes::dataTypeByName(const QString & name)
{
    list(); // init

    return _map.value(affinityByName(name));
}

} // namespace db
} // namespace meow
