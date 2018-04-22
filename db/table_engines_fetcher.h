#ifndef DB_TABLE_ENGINES_FETCHER_H
#define DB_TABLE_ENGINES_FETCHER_H

#include <QStringList>
#include <QPair>

namespace meow {
namespace db {

class Connection;

class TableEnginesFetcher
{
public:
    TableEnginesFetcher(Connection * connection)
        : _connection(connection),
          _list(false, {}) {}

    virtual ~TableEnginesFetcher() {}

    virtual QStringList run() = 0;

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

    // TODO: default engine ?

protected:
    Connection * _connection;
private:
    QPair<bool, QStringList> _list;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENGINES_FETCHER_H
