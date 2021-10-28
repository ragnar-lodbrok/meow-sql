#ifndef DB_USER_QUERY_H
#define DB_USER_QUERY_H

#include <atomic>
#include <QStringList>
#include <QVector>
#include "db/query_data.h"
#include "threads/helpers.h"

namespace meow {
namespace threads {
class QueryTask;
}
namespace db {

class ConnectionsManager;

// Arbitrary user query(ies)
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
        return _resultsData[index];
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

    Q_SIGNAL void finished();
    Q_SIGNAL void isRunningChanged(bool isRunning);

private:

    Q_SLOT void onQueryTaskFinished();

    QString generateUniqueId() const;

    ConnectionsManager * _connectionsManager;
    QVector<QueryDataPtr> _resultsData;
    QString _currentQueryText;
    mutable QString _uniqieId;
    bool _modifiedButNotSaved;
    std::shared_ptr<threads::QueryTask> _queryTask;
    std::atomic<bool> _isRunning;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_H
