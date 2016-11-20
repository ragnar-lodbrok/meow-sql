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

    virtual void execute(bool addResult = false, std::size_t useRawResult = -1) override;

    virtual bool hasResult() override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index, bool ignoreErrors = false) override;

private:
    std::vector<MySQLResult> _resultList;
    MYSQL_ROW _curRow;
    MySQLResult _currentResult; // TODO: really need this? H: FCurrentResults
    std::vector<unsigned int> _columnLengths; // FColumnLengths
};

} // namespace db
} // namespace meow

#endif // DB_MYSQLQUERY_H
