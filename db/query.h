#ifndef DB_QUERY_H
#define DB_QUERY_H

#include <QString>
#include <QStringList>
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

    db::ulonglong recordCount() const { return _recordCount; }
    bool isEof() const { return _eof; }

    std::size_t columnCount() const { return _columnNames.size(); }

    // H: procedure Execute(AddResult: Boolean=False; UseRawResult: Integer=-1); virtual; abstract;
    virtual void execute(bool addResult = false, std::size_t useRawResult = -1) = 0;

    virtual bool hasResult() = 0;

    virtual void seekRecNo(db::ulonglong value) = 0; // H: SetRecNo

    virtual QString curRowColumn(std::size_t index, bool ignoreErrors = false) = 0;

    void seekFirst();
    void seekNext();

protected:
    db::ulonglong _recordCount;
    db::ulonglong  _curRecNo; // H: FRecNo
    QStringList _columnNames;
    QStringList _columnOrgNames;
    bool _eof; // H: FEof

private:
    QString _SQL;
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_H
