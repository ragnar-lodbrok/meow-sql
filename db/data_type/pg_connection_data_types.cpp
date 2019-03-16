#include "pg_connection_data_types.h"
#include "db/pg/pg_connection.h"

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

        // TODO: fill list

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

} // namespace db
} // namespace meow

