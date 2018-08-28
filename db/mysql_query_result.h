#ifndef DB_MYSQL_QUERY_RESULT_H
#define DB_MYSQL_QUERY_RESULT_H

#include "native_query_result_interface.h"
#include <mysql/mysql.h>
#include <QDebug>
namespace meow {
namespace db {

// Wraps and owns MYSQL_RES ptr
class MySQLQueryResult : public INativeQueryResultInterface
{
public:
    MySQLQueryResult(MYSQL_RES * res) :
        INativeQueryResultInterface(),
        _res(res) {}

    virtual ~MySQLQueryResult() {
        mysql_free_result(_res);
    }

    MYSQL_RES * nativePtr() { return _res; }

private:
    MYSQL_RES * _res;
};

using MySQLQueryResultPt = std::shared_ptr<MySQLQueryResult>;

} // namespace db
} // namespace meow

#endif // DB_MYSQL_QUERY_RESULT_H
