#ifndef MYSQL_COLLATION_FETCHER_H
#define MYSQL_COLLATION_FETCHER_H

#include "collation_fetcher.h"

namespace meow {
namespace db {

class MySQLCollationFetcher : public CollationFetcher
{
public:
    MySQLCollationFetcher(Connection * connection);
    virtual QStringList run() override;
};

} // namespace db
} // namespace meow

#endif // MYSQL_COLLATION_FETCHER_H
