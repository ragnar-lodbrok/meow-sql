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

    const QStringList getList(bool refresh = false) {
        if (_list.first && refresh == false) {
            // return cached
            return _list.second;
        }
        _list.second = run();
        _list.first = true;
        return _list.second;
    }

protected:
    Connection * _connection;
private:
    QPair<bool, QStringList> _list;
};

} // namespace db
} // namespace meow

#endif // DB_COLLATION_FETCHER_H
