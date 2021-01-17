#ifndef DB_MYSQL_CONNECTION_DATA_TYPES_H
#define DB_MYSQL_CONNECTION_DATA_TYPES_H

// https://www.sqlite.org/datatype3.html
// https://doc.qt.io/qt-5/sql-types.html#qsqlite-sqlite-version-3-data-types

#include "connection_data_types.h"
#include <mysql/mysql.h>

namespace meow {
namespace db {

class MySQLConnection;

class MySQLConnectionDataTypes : public ConnectionDataTypes
{
public:
    MySQLConnectionDataTypes(MySQLConnection * connection);

    virtual const QList<DataTypePtr> & list() override;

    virtual const DataTypePtr defaultType() const override;

    virtual bool isDateTimeType(const DataTypePtr & type) const override;

    DataTypePtr dataTypeOfField(MYSQL_FIELD * field);

private:

    DataTypePtr dataTypeFromNative(const int nativeDatatype,
                                   bool isBinary = false);

    DataTypeIndex dataTypeIndexFromNative(const int nativeDatatype,
                                          bool isBinary);

    bool _init;
    QList<DataTypePtr> _list; // to be sorted
    QMap<DataTypeIndex, DataTypePtr> _map; // for search
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_CONNECTION_DATA_TYPES_H
