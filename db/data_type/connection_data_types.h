#ifndef DB_CONNECTION_DATA_TYPES_H
#define DB_CONNECTION_DATA_TYPES_H

#include <QList>
#include "data_type.h"

namespace meow {
namespace db {

class Connection;

class ConnectionDataTypes
{
public:
    explicit ConnectionDataTypes(Connection * connection)
        :_connection(connection)
    {

    }

    virtual ~ConnectionDataTypes() {}

    virtual const QList<DataTypePtr> & list() = 0;

protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_DATA_TYPES_H
