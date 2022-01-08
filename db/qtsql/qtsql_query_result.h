#ifndef DB_QTSQL_QUERY_RESULT_H
#define DB_QTSQL_QUERY_RESULT_H

#include <QtSql>

#include "db/native_query_result.h"
#include "db/common.h"

namespace meow {
namespace db {

class QtSQLQueryResult : public NativeQueryResult
{
public:
    QtSQLQueryResult(Connection * connection = nullptr)
        : NativeQueryResult(connection)
        , _query(nullptr)
        , _database(nullptr)
        , _rowsCountCache(static_cast<db::ulonglong>(-1))
        , _columnsParsed(false)
        , _currentQuery(nullptr)
    {

    }

    void init(QSqlQuery * query, QSqlDatabase * database);

    virtual ~QtSQLQueryResult() override {
        delete _query;
    }

    virtual db::ulonglong nativeRowsCount() const override;

    virtual bool hasData() const override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) override;

    virtual bool isNull(std::size_t index) override;

    QSqlQuery * query() const {
        return _query;
    }
protected:
    virtual void prepareResultForEditing(NativeQueryResult * result) override;
private:

    void clearColumnData();
    void addColumnData(QSqlQuery * query);
    std::vector<const QtSQLQueryResult *> resultList() const;

    QSqlQuery * _query;
    QSqlDatabase * _database;
    mutable db::ulonglong _rowsCountCache;
    bool _columnsParsed;
    QSqlQuery * _currentQuery;
};

using QtSQLQueryResultPtr = std::shared_ptr<QtSQLQueryResult>;

} // namespace db
} // namespace meow


#endif // DB_QTSQL_QUERY_RESULT_H
