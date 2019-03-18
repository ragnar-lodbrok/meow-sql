#ifndef DB_PG_QUERY_H
#define DB_PG_QUERY_H

#include "db/query.h"
#include "pg_connection.h"
#include "pg_query_result.h"

namespace meow {
namespace db {

class PGQuery : public Query
{
public:
    PGQuery(Connection * connection = nullptr);
    virtual ~PGQuery() override;

    virtual void execute(bool addResult = false) override;

    virtual bool hasResult() override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) override;

    virtual bool isNull(std::size_t index) override;

    virtual bool prepareEditing() override;

private:

    QString rowDataToString(PGresult * result,
                            int row,
                            int col,
                            int dataLen);

    void clearColumnData();
    void addColumnData(PGQueryResultPtr & result);

    std::vector<PGQueryResultPtr> _resultList;
    PGQueryResultPtr _currentResult;
    db::ulonglong  _curRecNoLocal;
    std::vector<unsigned int> _columnLengths;
    bool _columnsParsed;
};

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_H
