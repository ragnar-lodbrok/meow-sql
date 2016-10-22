#ifndef DB_QUERY_H
#define DB_QUERY_H

#include <QString>

namespace meow {
namespace db {

class Connection;

class Query
{
public:
    Query(Connection * connection);
    virtual ~Query();

    const QString & SQL() const { return _SQL; }
    Connection * connection() const { return _connection; }

    // H: procedure Execute(AddResult: Boolean=False; UseRawResult: Integer=-1); virtual; abstract;
    virtual void execute(bool addResult = false, int useRawResult = -1) = 0;

private:
    QString _SQL;
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_H
