#ifndef DB_USER_QUERY_H
#define DB_USER_QUERY_H

#include <atomic>
#include <QStringList>
#include <QVector>
#include "db/query_data.h"
#include "threads/helpers.h"

namespace meow {
namespace threads {
class QueriesTask;
}
namespace db {

class ConnectionsManager;

// Arbitrary user query(ies)
// TODO: rename to UserQueries?
class UserQuery : public QObject
{
    Q_OBJECT
public:
    explicit UserQuery(ConnectionsManager * connectionsManager);
    ~UserQuery() override;

    void runInCurrentConnection(const QStringList & queries);
    QString lastError() const;

    int resultsDataCount() const {
        MEOW_ASSERT_MAIN_THREAD
        return _resultsData.length();
    }
    QueryDataPtr resultsDataAt(int index) const {
        MEOW_ASSERT_MAIN_THREAD
        return _resultsData.at(index);
    }
    void setCurrentQueryText(const QString & query) {
        MEOW_ASSERT_MAIN_THREAD
        _currentQueryText = query;
    }
    QString currentQueryText() const {
        MEOW_ASSERT_MAIN_THREAD
        return _currentQueryText;
    }
    void onQueryTextEdited(const QString & query) { // by user
        MEOW_ASSERT_MAIN_THREAD
        bool modified = (_modifiedButNotSaved == false)
                && _currentQueryText != query; // avoid comparing long text
        _currentQueryText = query;
        if (modified) {
            setModifiedButNotSaved(modified);
        }
    }

    void setUniqueId(const QString & id) {
        MEOW_ASSERT_MAIN_THREAD
        _uniqieId = id;
    }
    QString uniqueId() const;

    bool modifiedButNotSaved() const {
        return _modifiedButNotSaved;
    }
    void setModifiedButNotSaved(bool modified) {
        _modifiedButNotSaved = modified;
    }

    bool isRunning() const {
        return _isRunning;
    }

    void setIsRunning(bool isRunning);

    Q_SIGNAL void queryFinished(int queryIndex, int totalCount);
    Q_SIGNAL void queriesFinished();
    Q_SIGNAL void newQueryDataResult(int index);
    Q_SIGNAL void isRunningChanged(bool isRunning);

private:

    Q_SLOT void onQueriesFinished();
    Q_SLOT void onQueryFinished(int queryIndex, int totalCount);

    QString generateUniqueId() const;

    ConnectionsManager * _connectionsManager;
    QVector<QueryDataPtr> _resultsData;
    QString _currentQueryText;
    mutable QString _uniqieId;
    bool _modifiedButNotSaved;
    std::shared_ptr<threads::QueriesTask> _queriesTask;
    std::atomic<bool> _isRunning;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_H
