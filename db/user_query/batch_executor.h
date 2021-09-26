#ifndef DB_USER_QUERY_BATCH_EXECUTOR_H
#define DB_USER_QUERY_BATCH_EXECUTOR_H

#include <QStringList>
#include <QList>
#include "db/connection.h"

namespace meow {
namespace db {

namespace user_query {

class BatchExecutor
{
public:
    BatchExecutor();
    bool run(Connection * connection, const QStringList & queries);

    const QList<db::QueryPtr> & results() const { return _results; }
    const db::Exception & error() const { return _error; }
    bool failed() const { return _failed; }

private:
    QList<db::QueryPtr> _results;
    db::Exception _error;
    bool _failed;
};

} // namespace user_query
} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_BATCH_EXECUTOR_H
