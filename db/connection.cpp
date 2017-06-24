#include "connection.h"
#include "query.h"
#include "entity/entities_fetcher.h"

namespace meow {
namespace db {

Connection::Connection(const ConnectionParameters & params)
    ://QObject(nullptr),
      _active(false),
     _connectionParams(params),
     _characterSet(),
     _isUnicode(false)
{

}

Connection::~Connection()
{
    // H: clears
    qDeleteAll(_databaseEntitiesCache);
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
    if (_allDatabasesCached.first == false || refresh) { // cache is empty or F5
        _allDatabasesCached.second = fetchDatabases();
        _allDatabasesCached.first = true;
    }

    return _allDatabasesCached.second;
}

EntityListForDataBase * Connection::getDbEntities(const QString & dbName, bool refresh /*= false*/)
{
    bool hasInCache = _databaseEntitiesCache.contains(dbName);

    if (refresh && hasInCache) {
        auto list = _databaseEntitiesCache.value(dbName);
        _databaseEntitiesCache.remove(dbName);
        delete list;
        hasInCache = false;
    }

    if (hasInCache) {
        return _databaseEntitiesCache.value(dbName);
    } else {
        // fetch
        EntityListForDataBase * newList = new EntityListForDataBase();
        _databaseEntitiesCache.insert(dbName, newList);

        DataBaseEntitiesFetcher * fetcherPtr = createDbEntitiesFetcher();

        std::shared_ptr<DataBaseEntitiesFetcher> fetcher(fetcherPtr);

        fetcherPtr->run(dbName, newList);

        return newList;
    }
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

QString Connection::quoteIdentifier(const char * identifier, bool alwaysQuote /*= true*/, QChar glue /*= QChar::Null*/) const
{
    QString id(identifier);
    return quoteIdentifier(id, alwaysQuote, glue);
}

QString Connection::quoteIdentifier(const QString & identifier, bool alwaysQuote /*= true*/, QChar glue /*= QChar::Null*/) const
{
    if (glue != QChar::Null) {
        // TODO
    }

    if (alwaysQuote) {

        QLatin1Char quoteChar('`');
        QString id(identifier);
        QString quoteReplaced = id.replace(quoteChar, "``"); // TODO: H: diff chars for diff db types
        return quoteChar + quoteReplaced + quoteChar; // TODO: replace "." with "`.`"
    } else {
        // TODO
    }

    return identifier;
}

void Connection::emitDatabaseChanged(const QString& newName)
{
    emit databaseChanged(newName);
}

} // namespace db
} // namespace meow
