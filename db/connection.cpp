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

QStringList Connection::allDatabases()
{
    return fetchDatabases();
}

// virtual
void Connection::setCharacterSet(const QString & characterSet)
{
   _characterSet = characterSet;
}

QStringList Connection::getColumn(const QString & SQL, int index)
{
    QueryPtr query = getResults(SQL);

    Q_UNUSED(index);

    return QStringList();
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
