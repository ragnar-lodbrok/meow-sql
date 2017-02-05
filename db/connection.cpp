#include "connection.h"
#include "query.h"

namespace meow {
namespace db {

Connection::Connection(const ConnectionParameters & params)
    :_active(false),
     _connectionParams(params),
     _characterSet(),
     _isUnicode(false)
{

}

Connection::~Connection()
{
    // H: clears
}

void Connection::doBeforeConnect()
{
    // TODO
    // H:
}

void Connection::doAfterConnect()
{
    // TODO
    // H:
}

QStringList Connection::allDatabases(bool refresh /*= false */)
{
    if (_allDatabasesCashed.first == false || refresh) { // cache is empty or F5
        _allDatabasesCashed.second = fetchDatabases();
        _allDatabasesCashed.first = true;
    }

    return _allDatabasesCashed.second;
}

// virtual
void Connection::setCharacterSet(const QString & characterSet)
{
   _characterSet = characterSet;
}

QStringList Connection::getColumn(const QString & SQL, std::size_t index)
{
    QueryPtr query = getResults(SQL);

    QStringList result;

    Query * queryPtr = query.get();

    if (queryPtr->recordCount() > 0) {
        while (queryPtr->isEof() == false) {
            result.append(queryPtr->curRowColumn(index));
            queryPtr->seekNext();
        }
    }

    return result;
}

QString Connection::getCell(const QString & SQL, std::size_t index /*= 0*/)
{
    QueryPtr query = getResults(SQL);

    Query * queryPtr = query.get();

    if (queryPtr->recordCount() > 0) {
        return queryPtr->curRowColumn(index);
    }

    return QString();
}

QueryPtr Connection::getResults(const QString & SQL)
{
    QueryPtr query = createQuery();
    query->setSQL(SQL);

    query->execute();

    return query;
}

QString Connection::quoteIdentifier(const char * identifier, bool alwaysQuote /*= true*/, QChar glue /*= QChar::Null*/)
{
    QString id(identifier);
    return quoteIdentifier(id, alwaysQuote, glue);
}

QString Connection::quoteIdentifier(QString & identifier, bool alwaysQuote /*= true*/, QChar glue /*= QChar::Null*/)
{
    if (glue != QChar::Null) {
        // TODO
    }

    if (alwaysQuote) {

        QChar quoteChar('`');

        QString quoteReplaced = identifier.replace(quoteChar, "``"); // TODO: H: diff chars for diff db types
        return quoteChar + quoteReplaced + quoteChar;
    } else {
        // TODO
    }

    return identifier;
}

} // namespace db
} // namespace meow
