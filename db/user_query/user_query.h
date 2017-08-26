#ifndef DB_USER_QUERY_H
#define DB_USER_QUERY_H

#include <QStringList>
#include <QVector>
#include "batch_executor.h"

namespace meow {
namespace db {

class ConnectionsManager;
class QueryData;

// Arbitrary user query(ies)
class UserQuery
{
public:
    UserQuery(ConnectionsManager * connectionsManager);
    ~UserQuery();

    bool runInCurrentConnection(const QStringList & queries);
    QString lastError() const;

    int resultsDataCount() const { return _resultsData.length(); }

private:
    ConnectionsManager * _connectionsManager;
    user_query::BatchExecutor _executor;
    QVector<QueryData *> _resultsData;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_H
