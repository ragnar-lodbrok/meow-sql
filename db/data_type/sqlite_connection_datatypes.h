#ifndef DB_QTSQL_CONNECTION_DATATYPES_H
#define DB_QTSQL_CONNECTION_DATATYPES_H

#include "connection_data_types.h"

namespace meow {
namespace db {

enum class SQLiteTypeAffinity : int
{
    // see https://www.sqlite.org/datatype3.html#type_affinity
    Text,
    Numeric,
    Integer,
    Real,
    Blob
};

class SQLiteConnection;

class SQLiteConnectionDataTypes : public ConnectionDataTypes
{
public:
    explicit SQLiteConnectionDataTypes(SQLiteConnection * connection);

    virtual const QList<DataTypePtr> & list() override;

    virtual const DataTypePtr defaultType() const override;

    virtual DataTypePtr dataTypeFromField(const QSqlField & field) override;

    SQLiteTypeAffinity affinityByName(const QString & name);

    DataTypePtr dataTypeByName(const QString & name);

private:
    bool _init;
    QList<DataTypePtr> _list; // to be sorted
    QMap<SQLiteTypeAffinity, DataTypePtr> _map; // for search
};

} // namespace db
} // namespace meow


#endif // DB_QTSQL_CONNECTION_DATATYPES_H
