#include "user_query.h"
#include "db/connections_manager.h"
#include "db/query_data.h"
#include "threads/db_thread.h"
#include "threads/query_task.h"
#include <QUuid>

namespace meow {
namespace db {

UserQuery::UserQuery(ConnectionsManager * connectionsManager)
    : QObject(nullptr)
    , _connectionsManager(connectionsManager)
    , _modifiedButNotSaved(false)
    , _isRunning(false)
{

}

UserQuery::~UserQuery()
{
    // Handle possible case when task is not finished, but we are dead
    setIsRunning(false);
    // TODO: emit finished/cancelled if destroyed running query
}

void UserQuery::runInCurrentConnection(const QStringList & queries)
{
    //setCurrentQueryText(queries.join(QChar::LineFeed)); // keep ?

    MEOW_ASSERT_MAIN_THREAD

    // do ping in main thread to handle possible reconnection
    try {
        _connectionsManager->activeConnection()->ping(true);
    } catch(meow::db::Exception & ex) {
        Q_UNUSED(ex);
        // TODO: process exception?
    }

    Q_ASSERT(isRunning() == false); // allow 1 query, block outside

    setIsRunning(true);

    threads::DbThread * thread
            = _connectionsManager->activeConnection()->thread();
    _queryTask = thread->createQueryTask(queries);
    connect(_queryTask.get(), &threads::ThreadTask::finished,
            this, &UserQuery::onQueryTaskFinished); // before post!
    thread->postTask(_queryTask);
}

QString UserQuery::lastError() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queryTask ? _queryTask->errorMessage() : QString();
}

QString UserQuery::uniqueId() const
{
    MEOW_ASSERT_MAIN_THREAD
    if (_uniqieId.isEmpty()) {
        _uniqieId = generateUniqueId();
    }
    return _uniqieId;
}

void UserQuery::onQueryTaskFinished()
{
    MEOW_ASSERT_MAIN_THREAD
    _resultsData.clear();

    const QList<db::QueryPtr> & resultsData = _queryTask->results();
    _resultsData.reserve(resultsData.length());
    QList<db::QueryPtr>::const_iterator i;
    for (i = resultsData.begin(); i != resultsData.end(); ++i) {
        QueryDataPtr queryData(new QueryData());
        queryData->setQueryPtr(*i);
        _resultsData.append(queryData);
    }

    setIsRunning(false);

    emit finished();
}

void UserQuery::setIsRunning(bool isRunning)
{
    bool prev = _isRunning.exchange(isRunning);
    if (prev != isRunning) {
        emit isRunningChanged(isRunning);
    }
}

QString UserQuery::generateUniqueId() const
{
    QUuid uid = QUuid::createUuid();
    QString uidStr = uid.toString();
    return uidStr.mid(1, uidStr.length()-2); // trim brackets
}

} // namespace db
} // namespace meow
