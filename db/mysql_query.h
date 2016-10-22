#ifndef DB_MYSQLQUERY_H
#define DB_MYSQLQUERY_H

#include "query.h"

namespace meow {
namespace db {

class MySQLQuery : public Query
{
public:
    MySQLQuery(Connection * connection);
    virtual ~MySQLQuery();

    virtual void execute(bool addResult = false, int useRawResult = -1) override;

};

} // namespace db
} // namespace meow

#endif // DB_MYSQLQUERY_H
