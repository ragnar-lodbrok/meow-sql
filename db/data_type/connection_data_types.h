#ifndef DB_CONNECTION_DATA_TYPES_H
#define DB_CONNECTION_DATA_TYPES_H

#include <QList>
#ifdef WITH_QTSQL
#include <QSqlField>
#endif
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

    virtual const DataTypePtr defaultType() const = 0;

    virtual bool isDateTimeType(const DataTypePtr & type) const {
        Q_UNUSED(type);
        return false;
    }

    DataTypePtr createUnknownType() const { // rm, just use default cted
        DataTypePtr ptr(
            new DataType(
                DataTypeIndex::Unknown,
                -1,
                "UNKNOWN",
                false, // hasLength
                DataTypeCategoryIndex::Other
            )
        );
        return ptr;
    }
#ifdef WITH_QTSQL
    virtual DataTypePtr dataTypeFromField(const QSqlField & field) {
        Q_UNUSED(field)
        return defaultType();
    }
#endif

protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_DATA_TYPES_H
