#ifndef DB_MYSQLQUERY_H
#define DB_MYSQLQUERY_H

#include "query.h"
#include "mysql_connection.h"

namespace meow {
namespace db {

class MySQLQuery : public Query
{
public:
    MySQLQuery(Connection * connection = nullptr);
    virtual ~MySQLQuery();

    virtual void execute(bool addResult = false, int useRawResult = -1) override;

    virtual bool hasResult() override;

private:
    std::vector<MySQLResult> _resultList;

};

} // namespace db
} // namespace meow

#endif // DB_MYSQLQUERY_H
