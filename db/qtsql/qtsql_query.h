#ifndef DB_QTSQL_QUERY_H
#define DB_QTSQL_QUERY_H

#include "db/query.h"
#include "qtsql_query_result.h"

namespace meow {
namespace db {

// Intent: Query for Qt SQL
class QtSQLQuery : public Query
{
public:

    explicit QtSQLQuery(Connection * connection = nullptr);
    virtual ~QtSQLQuery() override;

    virtual void execute(bool addResult = false) override;

    virtual bool hasResult() override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) override;

    virtual bool isNull(std::size_t index) override;

    virtual bool prepareEditing() override;

private:

    void clearColumnData();
    void addColumnData(QtSQLQueryResultPtr & result);

    std::vector<QtSQLQueryResultPtr> _resultList;
    QtSQLQueryResultPtr _currentResult;
    db::ulonglong  _curRecNoLocal;
    bool _columnsParsed;
};

} // namespace db
} // namespace meow

#endif // DB_QTSQL_QUERY_H
