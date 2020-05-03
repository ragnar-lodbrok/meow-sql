#ifndef MYSQL_COLLATION_FETCHER_H
#define MYSQL_COLLATION_FETCHER_H

#include "db/collation_fetcher.h"

namespace meow {
namespace db {

class MySQLCollationFetcher : public CollationFetcher
{
public:
    explicit MySQLCollationFetcher(Connection * connection);
    virtual QStringList run() override;
    virtual QString fetchServerDefaultCollation() override;
    virtual QString serverPrefferedCollation() override;
};

} // namespace db
} // namespace meow

#endif // MYSQL_COLLATION_FETCHER_H
