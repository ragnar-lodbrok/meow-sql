#include "connection.h"
#include "query.h"
#include "entity/entities_fetcher.h"
#include "table_editor.h"
#include "database_editor.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "table_engines_fetcher.h"
#include "query_data_editor.h"

namespace meow {
namespace db {

Connection::Connection(const ConnectionParameters & params)
    ://QObject(nullptr),
     _active(false),
     _identifierQuote('`'),
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

void Connection::setAllDatabases(const QStringList & databases)
{
    _allDatabasesCached.first = true;
    _allDatabasesCached.second = databases;
}

void Connection::setUseAllDatabases()
{
    _allDatabasesCached.first = false;
    _allDatabasesCached.second = QStringList();
}

EntityListForDataBase * Connection::getDbEntities(const QString & dbName,
                                                  bool refresh /*= false*/)
{
    bool hasInCache = _databaseEntitiesCache.contains(dbName);

    if (refresh && hasInCache) {
        deleteAllCachedEntitiesInDatabase(dbName);
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

bool Connection::deleteAllCachedEntitiesInDatabase(const QString & dbName)
{
    if (_databaseEntitiesCache.contains(dbName)) {
        auto list = _databaseEntitiesCache.value(dbName);
        _databaseEntitiesCache.remove(dbName);
        delete list;
        return true;
    }
    return false;
}

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

QString Connection::getCell(const QString & SQL,const QString & columnName)
{
    QueryPtr query = getResults(SQL);

    Query * queryPtr = query.get();

    if (queryPtr->recordCount() > 0) {
        return queryPtr->curRowColumn(columnName);
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

QStringList Connection::getRow(const QString & SQL)
{
    // TODO: say query to skip columns parsing
    QueryPtr query = getResults(SQL);
    query->seekFirst();
    return query->curRow();
}

QString Connection::quoteIdentifier(const char * identifier,
                                    bool alwaysQuote /*= true*/,
                                    QChar glue /*= QChar::Null*/) const
{
    QString id(identifier);
    return quoteIdentifier(id, alwaysQuote, glue);
}

QString Connection::quoteIdentifier(const QString & identifier,
                                    bool alwaysQuote /*= true*/,
                                    QChar glue /*= QChar::Null*/) const
{
    if (glue != QChar::Null) {
        // TODO
    }

    if (alwaysQuote) {

        QString doubleQuote;
        doubleQuote.append(_identifierQuote);
        doubleQuote.append(_identifierQuote);
        QString id(identifier);
        QString quoteReplaced = id.replace(_identifierQuote, doubleQuote);
        // TODO: H: diff chars for diff db types
        return _identifierQuote + quoteReplaced + _identifierQuote;
        // TODO: replace "." with "`.`"
    } else {
        // TODO
    }

    return identifier;
}

QStringList Connection::quoteIdentifiers(const QStringList & identifiers) const
{
    QStringList result;

    for (const QString & id : identifiers) {
        result << quoteIdentifier(id);
    }

    return result;
}

const QStringList Connection::collationList()
{
    if (_collationFetcher == nullptr) {
        _collationFetcher.reset(createCollationFetcher());
    }

    return _collationFetcher->getList();
}

QString Connection::serverDefaultCollation()
{
    if (_collationFetcher == nullptr) {
        _collationFetcher.reset(createCollationFetcher());
    }
    return _collationFetcher->serverDefaultCollation();
}

QString Connection::serverPrefferedCollation()
{
    if (_collationFetcher == nullptr) {
        _collationFetcher.reset(createCollationFetcher());
    }
    return _collationFetcher->serverPrefferedCollation();
}

const QStringList Connection::tableEnginesList()
{
    if (_tableEnginesFetcher == nullptr) {
        _tableEnginesFetcher.reset(createTableEnginesFetcher());
    }
    return _tableEnginesFetcher->getList();
}

QString Connection::defaultTableEngine()
{
    if (_tableEnginesFetcher == nullptr) {
        _tableEnginesFetcher.reset(createTableEnginesFetcher());
    }
    return _tableEnginesFetcher->defaultEngine();
}

void Connection::emitDatabaseChanged(const QString& newName)
{
    emit databaseChanged(newName);
}

void Connection::parseTableStructure(TableEntity * table, bool refresh)
{
    if (table->isNew()) return;
    if (!refresh && table->hasStructure()) {
        return;
    }
    _tableStructureParser.run(table);
}

bool Connection::editTableInDB(TableEntity * table, TableEntity * newData)
{

    TableEditor * editor = createTableEditor();

    std::shared_ptr<TableEditor> sharedEditor(editor);

    return sharedEditor->edit(table, newData);
}

bool Connection::insertTableToDB(TableEntity * table)
{
    TableEditor * editor = createTableEditor();

    std::shared_ptr<TableEditor> sharedEditor(editor);

    return sharedEditor->insert(table);
}

bool Connection::dropEntityInDB(EntityInDatabase * entity)
{
    TableEditor * editor = createTableEditor();

    std::shared_ptr<TableEditor> sharedEditor(editor);

    return sharedEditor->drop(entity);
}

bool Connection::dropDatabase(DataBaseEntity * database)
{
    DataBaseEditor * editor = createDataBaseEditor();

    std::shared_ptr<DataBaseEditor> sharedEditor(editor);

    return sharedEditor->drop(database->name());
}

void Connection::createDatabase(const QString & name,
                                const QString & collation)
{
    DataBaseEditor * editor = createDataBaseEditor();

    std::shared_ptr<DataBaseEditor> sharedEditor(editor);

    return sharedEditor->create(name, collation);
}

std::shared_ptr<QueryDataEditor> Connection::queryDataEditor()
{
    return std::make_shared<QueryDataEditor>();
}

QString Connection::limitOnePostfix() const
{
    return ""; // some dbs dont have it
}

} // namespace db
} // namespace meow
