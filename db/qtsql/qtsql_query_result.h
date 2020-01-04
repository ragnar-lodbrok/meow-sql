#ifndef DB_QTSQL_QUERY_RESULT_H
#define DB_QTSQL_QUERY_RESULT_H

#include <QtSql>

#include "db/native_query_result_interface.h"
#include "db/common.h"

namespace meow {
namespace db {

class QtSQLQueryResult : public INativeQueryResultInterface
{
public:
    QtSQLQueryResult(QSqlQuery * query, QSqlDatabase * database) :
        _query(query),
        _database(database),
        _rowsCount(static_cast<db::ulonglong>(-1))
    {

    }

    virtual ~QtSQLQueryResult() override {
        delete _query;
    }

    virtual db::ulonglong rowsCount() const override {

        if (_database->driver()->hasFeature(QSqlDriver::QuerySize)) { // fast

            return static_cast<db::ulonglong>(_query->size());

        } else { // slow
            if (_rowsCount == static_cast<db::ulonglong>(-1)) {
                _query->last();
                int currentRow = _query->at();
                if (currentRow < 0) { // on error this value is -2 in sqlite
                    _rowsCount = 0;
                } else {
                    _rowsCount = static_cast<db::ulonglong>(currentRow + 1);
                }

                // TODO: seek first?
                //_query->first();
            }
            return _rowsCount;
        }
    }

    QSqlQuery * query() const {
        return _query;
    }

private:

    QSqlQuery * _query;
    QSqlDatabase * _database;
    mutable db::ulonglong _rowsCount;
};

using QtSQLQueryResultPtr = std::shared_ptr<QtSQLQueryResult>;

} // namespace db
} // namespace meow


#endif // DB_QTSQL_QUERY_RESULT_H
