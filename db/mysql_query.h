#ifndef DB_MYSQLQUERY_H
#define DB_MYSQLQUERY_H

#include "query.h"
#include "mysql_connection.h"
#include "mysql_query_result.h"

namespace meow {
namespace db {

class MySQLQuery : public Query
{
public:
    MySQLQuery(Connection * connection = nullptr);
    virtual ~MySQLQuery() override;

    virtual void execute(bool addResult = false) override;

    virtual bool hasResult() override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) override;

    virtual bool isNull(std::size_t index) override;

    virtual bool prepareEditing() override;

private:

    DataTypeIndex dataTypeOfField(MYSQL_FIELD * field);
    void throwOnInvalidColumnIndex(std::size_t index);

    QString rowDataToString(MYSQL_ROW row,
                            std::size_t col,
                            unsigned long dataLen);

    void prepareResultForEditing(MYSQL_RES * result);

    void clearColumnData();
    void addColumnData(MYSQL_RES * result);


    std::vector<MySQLQueryResultPtr> _resultList;
    MYSQL_ROW _curRow;
    MySQLQueryResultPtr _currentResult; // TODO: really need this? H: FCurrentResults
    std::vector<unsigned int> _columnLengths; // FColumnLengths
    bool _columnsParsed;


};

} // namespace db
} // namespace meow

#endif // DB_MYSQLQUERY_H
