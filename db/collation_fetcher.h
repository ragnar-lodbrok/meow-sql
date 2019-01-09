#ifndef DB_COLLATION_FETCHER_H
#define DB_COLLATION_FETCHER_H

#include <QStringList>
#include <QPair>

namespace meow {
namespace db {

class Connection;

class CollationFetcher
{
public:
    CollationFetcher(Connection * connection)
        : _connection(connection),
          _list(false, {}) {}

    virtual ~CollationFetcher() {}

    virtual QStringList run() = 0;

    // what was setup
    QString serverDefaultCollation(bool refresh = false) {
        if (_serverDefaultCollation.first && refresh == false) {
            return _serverDefaultCollation.second;
        }
        _serverDefaultCollation.second = fetchServerDefaultCollation();
        _serverDefaultCollation.first = true;
        return _serverDefaultCollation.second;
    }

    virtual QString fetchServerDefaultCollation() {
        return QString();
    }

    // best, may be different from default
    virtual QString serverPrefferedCollation() {
        return serverDefaultCollation();
    }

    const QStringList getList(bool refresh = false) {
        if (_list.first && refresh == false) {
            // return cached
            return _list.second;
        }
        _list.second = run();
        _list.second.sort(Qt::CaseInsensitive);
        _list.first = true;
        return _list.second;
    }

protected:
    Connection * _connection;
private:
    QPair<bool, QStringList> _list;
    QPair<bool, QString> _serverDefaultCollation;
};

} // namespace db
} // namespace meow

#endif // DB_COLLATION_FETCHER_H
