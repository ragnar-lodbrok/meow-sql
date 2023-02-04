#include "user_query.h"
#include "db/connections_manager.h"
#include "db/query_data.h"
#include "threads/db_thread.h"
#include "threads/queries_task.h"
#include "helpers/logger.h"
#include "helpers/formatting.h"
#include <QUuid>

namespace meow {
namespace db {

UserQuery::UserQuery(ConnectionsManager * connectionsManager)
    : QObject(nullptr)
    , _connectionsManager(connectionsManager)
    , _lastRunningConnection(nullptr)
    , _modifiedButNotSaved(false)
    , _isRunning(false)
{

    connect(_connectionsManager, &ConnectionsManager::beforeConnectionClosed,
            this, &UserQuery::onConnectionClose);
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

    _lastRunningConnection = _connectionsManager->activeConnection();

    // do ping in main thread to handle possible reconnection
    try {
        _lastRunningConnection->ping(true);
         // get id before async query execution to allow
         // KILL QUERY ID from another thread
        _lastRunningConnection->connectionIdOnServer();
    } catch(meow::db::Exception & ex) {
        Q_UNUSED(ex);
        // TODO: process exception?
    }

    Q_ASSERT(isRunning() == false); // allow 1 query, block outside

    setIsRunning(true);

    _resultsData.clear();

    threads::DbThread * thread = _lastRunningConnection->thread();
    _queriesTask = thread->createQueriesTask(queries);

    connect(_queriesTask.get(), &threads::ThreadTask::finished,
            this, &UserQuery::onQueriesFinished); // before post!

    connect(_queriesTask.get(), &threads::QueriesTask::queryFinished,
            this, &UserQuery::onQueryFinished);

    thread->postTask(_queriesTask);
}

QString UserQuery::lastError() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask ? _queriesTask->errorMessage() : QString();
}

int UserQuery::queryTotalCount() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask ? _queriesTask->queryTotalCount() : 0;
}

int UserQuery::queryFailedCount() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask ? _queriesTask->queryFailedCount() : 0;
}

int UserQuery::querySuccessCount() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask ? _queriesTask->querySuccessCount() : 0;
}

std::chrono::milliseconds UserQuery::execDuration() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask
            ? _queriesTask->execDuration()
            : std::chrono::milliseconds(0);
}

std::chrono::milliseconds UserQuery::networkDuration() const
{
    MEOW_ASSERT_MAIN_THREAD
    return _queriesTask
            ? _queriesTask->networkDuration()
            : std::chrono::milliseconds(0);
}

QString UserQuery::uniqueId() const
{
    MEOW_ASSERT_MAIN_THREAD
    if (_uniqieId.isEmpty()) {
        _uniqieId = generateUniqueId();
    }
    return _uniqieId;
}

void UserQuery::onQueriesFinished()
{
    MEOW_ASSERT_MAIN_THREAD

    setIsRunning(false);

    emit queriesFinished();

    QStringList logStrings;

    logStrings << QObject::tr("Affected rows: %1")
                     .arg(_queriesTask->rowsAffected());


    logStrings << QObject::tr("Found rows: %1")
                      .arg(_queriesTask->rowsFound());

    if (_queriesTask->warningsCount() != 0) {
        logStrings << QObject::tr("Warnings: %1")
                          .arg(_queriesTask->warningsCount());
    }

    int totalCount = queryTotalCount();
    int successCount = querySuccessCount();

    QString durationAndCountStr = QObject::tr("Duration for %1 ").arg(successCount);

    if (successCount != totalCount) {
        durationAndCountStr += QObject::tr("of %1 ").arg(totalCount);
    }

    if (queryTotalCount() == 1) {
        durationAndCountStr += QObject::tr("query");
    } else {
        durationAndCountStr += QObject::tr("queries");
    }

    durationAndCountStr += QObject::tr(" %1 sec.").arg(
                helpers::formatAsSeconds(execDuration()));

    std::chrono::milliseconds networkDuration = this->networkDuration();

    if (networkDuration.count() > 0) {
        durationAndCountStr += QObject::tr(" (+%1 sec. network)").arg(
                    helpers::formatAsSeconds(networkDuration));
    }

    logStrings << durationAndCountStr;

    meowLogC(Log::Category::Info) << logStrings.join(" ");

    // Listening: Hatebreed - I will be heard
}

void UserQuery::onQueryFinished(int queryIndex, int totalCount)
{
    MEOW_ASSERT_MAIN_THREAD

    db::QueryPtr query = _queriesTask->resultAt(queryIndex);
    size_t prevResultsCount = _resultsData.size();

    if (query->hasResult()) {
        // some queries may return multiple results
        for (size_t i = 0; i < query->resultCount(); ++i) {
            QueryDataPtr queryData(new QueryData());
            queryData->setQueryPtr(query);
            queryData->setResultIndex(i);
            _resultsData.append(queryData);
        }
    }

    emit queryFinished(queryIndex, totalCount);
    if (query->hasResult()) {
        for (size_t i = 0; i < query->resultCount(); ++i) {
            emit newQueryDataResult(prevResultsCount + i);
        }
    }
}

void UserQuery::setIsRunning(bool isRunning)
{
    bool prev = _isRunning.exchange(isRunning);
    if (prev != isRunning) {
        emit isRunningChanged(isRunning);
    }
}

void UserQuery::abort()
{
    if (_queriesTask) {
        _queriesTask->abort();
    }
}

void UserQuery::onConnectionClose(SessionEntity * session)
{
    if (_lastRunningConnection == session->connection()) {
        _lastRunningConnection = nullptr;
        emit executionConnectionClosed();
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
