#include "user_query.h"
#include "db/connections_manager.h"
#include "db/query_data.h"
#include "threads/db_thread.h"
#include "threads/queries_task.h"
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

    _resultsData.clear();

    threads::DbThread * thread
            = _connectionsManager->activeConnection()->thread();
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
    if (_queriesTask->rowsAffected() != (db::ulonglong)-1) {
        logStrings << QObject::tr("Affected rows: %1")
                          .arg(_queriesTask->rowsAffected());
    }

    if (_queriesTask->rowsFound() != (db::ulonglong)-1) {
        logStrings << QObject::tr("Found rows: %1")
                          .arg(_queriesTask->rowsFound());
    }

    if (_queriesTask->rowsFound() != (db::ulonglong)-1) {
        logStrings << QObject::tr("Warnings: %1")
                          .arg(_queriesTask->warningsCount());
    }

    if (!logStrings.isEmpty()) {

        qDebug() << logStrings.join(" ");
        //meowLogCC(Log::Category::SQL, this) << SQL; // TODO
    }

    // TODO:
    /*

  MetaInfo := _('Affected rows')+': '+FormatNumber(Thread.RowsAffected)+
    '  '+_('Found rows')+': '+FormatNumber(Thread.RowsFound)+
    '  '+_('Warnings')+': '+FormatNumber(Thread.WarningCount)+
    '  '+_('Duration for')+' ' + FormatNumber(Thread.BatchPosition);
  if Thread.BatchPosition < Thread.Batch.Count then
    MetaInfo := MetaInfo + ' ' + _('of') + ' ' + FormatNumber(Thread.Batch.Count);
  if Thread.Batch.Count = 1 then
    MetaInfo := MetaInfo + ' ' + _('query')
  else
    MetaInfo := MetaInfo + ' ' + _('queries');
  if Thread.QueryTime < 60*1000 then
    MetaInfo := MetaInfo + ': '+FormatNumber(Thread.QueryTime/1000, 3) +' ' + _('sec.')
  else
    MetaInfo := MetaInfo + ': '+FormatTimeNumber(Thread.QueryTime/1000, True);
  if Thread.QueryNetTime > 0 then
    MetaInfo := MetaInfo + ' (+ '+FormatNumber(Thread.QueryNetTime/1000, 3) +' ' + _('sec.') + ' ' + _('network') + ')';
  LogSQL(MetaInfo);
    */

    // Listening: Hatebreed - I will be heard
}

void UserQuery::onQueryFinished(int queryIndex, int totalCount)
{
    MEOW_ASSERT_MAIN_THREAD

    db::QueryPtr query = _queriesTask->resultAt(queryIndex);

    if (query->hasResult()) {
        QueryDataPtr queryData(new QueryData());
        queryData->setQueryPtr(query);
        _resultsData.append(queryData);
    }

    emit queryFinished(queryIndex, totalCount);
    if (query->hasResult()) {
        emit newQueryDataResult(_resultsData.size() - 1);
    }
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
