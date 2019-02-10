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
};

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_H
