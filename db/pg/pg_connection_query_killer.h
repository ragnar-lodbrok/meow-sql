#ifndef DB_PG_CONNECTION_QUERY_KILLER_H
#define DB_PG_CONNECTION_QUERY_KILLER_H

#include "db/connection_query_killer.h"

namespace meow {
namespace db {

class PGConnectionQueryKiller : public ConnectionQueryKiller
{
public:
    explicit PGConnectionQueryKiller(Connection * connection);

protected:
    virtual QString killQueryStatement() const override;
};

} // namespace db
} // namespace meo

#endif // DB_PG_CONNECTION_QUERY_KILLER_H
