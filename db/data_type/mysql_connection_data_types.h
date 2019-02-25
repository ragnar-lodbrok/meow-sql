#ifndef DB_MYSQL_CONNECTION_DATA_TYPES_H
#define DB_MYSQL_CONNECTION_DATA_TYPES_H

#include "connection_data_types.h"

namespace meow {
namespace db {

class MySQLConnection;

class MySQLConnectionDataTypes : public ConnectionDataTypes
{
public:
    MySQLConnectionDataTypes(MySQLConnection * connection);

    virtual const QList<DataTypePtr> & list() override;

private:
    bool _init;
    QList<DataTypePtr> _list;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_CONNECTION_DATA_TYPES_H
