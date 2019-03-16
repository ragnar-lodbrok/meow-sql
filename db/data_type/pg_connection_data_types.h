#ifndef DB_PG_CONNECTION_DATA_TYPES_H
#define DB_PG_CONNECTION_DATA_TYPES_H

#include "connection_data_types.h"
#include <postgresql/libpq-fe.h>

namespace meow {
namespace db {

class PGConnection;

class PGConnectionDataTypes : public ConnectionDataTypes
{
public:

    PGConnectionDataTypes(PGConnection * connection);

    virtual const QList<DataTypePtr> & list() override;

    DataTypePtr dataTypeFromNative(const Oid nativeDatatype);

private:

    bool _init;
    QList<DataTypePtr> _list; // to be sorted
    QMap<Oid, DataTypePtr> _map; // for search
};

} // namespace db
} // namespace meow

#endif // DB_PG_CONNECTION_DATA_TYPES_H
