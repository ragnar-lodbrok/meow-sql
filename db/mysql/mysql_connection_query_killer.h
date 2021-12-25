#ifndef DB_MYSQL_CONNECTION_QUERY_KILLER_H
#define DB_MYSQL_CONNECTION_QUERY_KILLER_H

#include "db/connection_query_killer.h"

namespace meow {
namespace db {

class MySQLConnectionQueryKiller : public ConnectionQueryKiller
{
public:
    explicit MySQLConnectionQueryKiller(Connection * connection);

protected:
    virtual QString killQueryStatement() const override;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_CONNECTION_QUERY_KILLER_H
