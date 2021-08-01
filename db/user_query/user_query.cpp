#include "user_query.h"
#include "db/connections_manager.h"
#include "db/query_data.h"
#include <QUuid>

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
    setCurrentQueryText(queries.join(QChar::LineFeed));

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

QString UserQuery::uniqueId() const
{
    if (_uniqieId.isEmpty()) {
        _uniqieId = generateUniqueId();
    }
    return _uniqieId;
}

QString UserQuery::generateUniqueId() const
{
    QUuid uid = QUuid::createUuid();
    QString uidStr = uid.toString();
    return uidStr.mid(1, uidStr.length()-2); // trim brackets
}

} // namespace db
} // namespace meow
