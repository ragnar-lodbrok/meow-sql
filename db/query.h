#ifndef DB_QUERY_H
#define DB_QUERY_H

#include <QString>
#include "common.h"

namespace meow {
namespace db {

class Connection;

class Query
{
public:
    Query(Connection * connection = nullptr);
    virtual ~Query();

    void setSQL(const QString & SQL);
    void setConnection(Connection * connection) { _connection = connection; }

    const QString & SQL() const { return _SQL; }
    Connection * connection() const { return _connection; }

    // H: procedure Execute(AddResult: Boolean=False; UseRawResult: Integer=-1); virtual; abstract;
    virtual void execute(bool addResult = false, int useRawResult = -1) = 0;

    virtual bool hasResult() = 0;

protected:
    db::ulonglong _recordCount;

private:
    QString _SQL;
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_H
