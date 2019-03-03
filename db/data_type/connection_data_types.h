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

    // list of all possible types for this connection
    // copy type and modify for your needs
    virtual const QList<DataTypePtr> & list() = 0;
    // TODO: return list of const DataTypes

    DataTypePtr createUnknownType() const {
        DataTypePtr ptr(
            new DataType(
                DataTypeIndex::Unknown,
                -1,
                "UNKNOWN",
                false, // hasLength
                DataTypeCategoryIndex::None
            )
        );
        return ptr;
    }

protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_DATA_TYPES_H
