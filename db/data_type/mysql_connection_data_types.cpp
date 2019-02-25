#include "mysql_connection_data_types.h"
#include "db/mysql_connection.h"

namespace meow {
namespace db {

MySQLConnectionDataTypes::MySQLConnectionDataTypes(MySQLConnection * connection)
    : ConnectionDataTypes(connection),
      _init(false)
{

}

const QList<DataTypePtr> & MySQLConnectionDataTypes::list()
{

    if (_init == false) {
        _init = true;
        //_list.append(std::make_shared<DataType>());
        // TODO
    }

    return _list;
}

} // namespace db
} // namespace meow
