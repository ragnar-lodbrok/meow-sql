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
    explicit UserQuery(ConnectionsManager * connectionsManager);
    ~UserQuery();

    bool runInCurrentConnection(const QStringList & queries);
    QString lastError() const;

    int resultsDataCount() const { return _resultsData.length(); }
    QueryData * resultsDataAt(int index) const { return _resultsData[index]; }

    void setCurrentQueryText(const QString & query) {
        _currentQueryText = query;
    }
    QString currentQueryText() const { return _currentQueryText; }

    void onQueryTextEdited(const QString & query) { // by user
        bool modified = (_modifiedButNotSaved == false)
                && _currentQueryText != query; // avoid comparing long text
        _currentQueryText = query;
        if (modified) {
            setModifiedButNotSaved(modified);
        }
    }

    void setUniqueId(const QString & id) {
        _uniqieId = id;
    }
    QString uniqueId() const;

    bool modifiedButNotSaved() const {
        return _modifiedButNotSaved;
    }

    void setModifiedButNotSaved(bool modified) {
        _modifiedButNotSaved = modified;
    }

private:

    QString generateUniqueId() const;

    ConnectionsManager * _connectionsManager;
    user_query::BatchExecutor _executor;
    QVector<QueryData *> _resultsData;
    QString _currentQueryText;
    mutable QString _uniqieId;
    bool _modifiedButNotSaved;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_H
