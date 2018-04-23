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

    // First will be default
    virtual QStringList run() = 0;

    const QStringList getList(bool refresh = false) {
        if (_list.first && refresh == false) {
            // return cached
            return _list.second;
        }
        _list.second = run();
        if (_list.second.size()) {
            _default = _list.second[0];
        }
        _list.second.sort(Qt::CaseInsensitive);
        _list.first = true;
        return _list.second;
    }

    QString defaultEngine() {
        if (!_list.first) {
            getList(false);
        }
        return _default;
    }


    // TODO: default engine ?

protected:
    Connection * _connection;
private:
    QPair<bool, QStringList> _list;
    QString _default;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENGINES_FETCHER_H
