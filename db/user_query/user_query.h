#ifndef DB_USER_QUERY_H
#define DB_USER_QUERY_H

#include <QStringList>
#include <QVector>
#include "db/query_data.h"

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

    bool runInCurrentConnection(const QStringList & queries);
    QString lastError() const;

    int resultsDataCount() const { return _resultsData.length(); }
    QueryDataPtr resultsDataAt(int index) const { return _resultsData[index]; }

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

    Q_SIGNAL void finished();

private:

    Q_SLOT void onQueryTaskFinished();

    QString generateUniqueId() const;

    ConnectionsManager * _connectionsManager;
    QVector<QueryDataPtr> _resultsData;
    QString _currentQueryText;
    mutable QString _uniqieId;
    bool _modifiedButNotSaved;
    std::shared_ptr<threads::QueryTask> _queryTask;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERY_H
