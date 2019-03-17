#ifndef DB_PG_ENTITIES_FETCHER_H
#define DB_PG_ENTITIES_FETCHER_H

#include "db/entity/entities_fetcher.h"

namespace meow {
namespace db {

class PGConnection;

class PGEntitiesFetcher : public DataBaseEntitiesFetcher
{
public:
    PGEntitiesFetcher(PGConnection * connection);

    virtual void run(const QString & dbName,
             EntityListForDataBase * toList) override;
};

} // namespace db
} // namespace meow

#endif // DB_PG_ENTITIES_FETCHER_H
