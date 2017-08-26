#include "user_query.h"
#include "db/connections_manager.h"
#include "db/query_data.h"

namespace meow {
namespace db {

UserQuery::UserQuery(ConnectionsManager * connectionsManager)
    :_connectionsManager(connectionsManager)
{

}

UserQuery::~UserQuery()
{
    qDeleteAll(_resultsData);
}

bool UserQuery::runInCurrentConnection(const QStringList & queries)
{
    qDeleteAll(_resultsData);
    _resultsData.clear();

    bool result = _executor.run(
                _connectionsManager->activeConnection(), queries);

    const QList<db::QueryPtr> & resultsData = _executor.results();
    _resultsData.reserve(resultsData.length());
    QList<db::QueryPtr>::const_iterator i;
    for (i = resultsData.begin(); i != resultsData.end(); ++i) {
        QueryData * queryData = new QueryData();
        queryData->setQueryPtr(*i);
        _resultsData.append(queryData);
    }

    return result;
}

QString UserQuery::lastError() const
{
    return _executor.error().message();
}

} // namespace db
} // namespace meow
