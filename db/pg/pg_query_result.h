#ifndef DB_PG_QUERY_RESULT_H
#define DB_PG_QUERY_RESULT_H

#include <postgresql/libpq-fe.h>
#include "db/native_query_result_interface.h"
#include "db/common.h"

namespace meow {
namespace db {

// Wraps and owns PGresult ptr
class PGQueryResult : public INativeQueryResultInterface
{
public:
    PGQueryResult(PGresult * res, PGconn * connection) :
        INativeQueryResultInterface(),
        _res(res),
        _connection(connection)
    {

    }

    virtual ~PGQueryResult() override {
        PQclear(_res);
    }

    virtual db::ulonglong rowsCount() const override {
        return static_cast<db::ulonglong>(PQntuples(_res));
    }

    void clearAll() {
        while (_res != nullptr) {
            PQclear(_res);
            _res = PQgetResult(_connection);
        }
    }

    PGresult * nativePtr() const { return _res; }

private:
    PGresult * _res;
    PGconn * _connection;
};

using PGQueryResultPtr = std::shared_ptr<PGQueryResult>;

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_RESULT_H
