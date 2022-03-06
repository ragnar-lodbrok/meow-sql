#include "connection.h"
#include "query.h"
#include "entity/entities_fetcher.h"
#include "table_editor.h"
#include "view_editor.h"
#include "routine_editor.h"
#include "database_editor.h"
#include "trigger_editor.h"
#include "db/entity/table_entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/view_entity.h"
#include "db/entity/routine_entity.h"
#include "db/entity/trigger_entity.h"
#include "table_engines_fetcher.h"
#include "query_data_editor.h"
#include "helpers/parsing.h"
#include "trigger_structure_parser.h"
#include "threads/db_thread.h"
#include "db_thread_initializer.h"
#include "connection_query_killer.h"

#include <QDebug>

namespace meow {
namespace db {

Connection::Connection(const ConnectionParameters & params)
    : _mutex(!params.supportsMultithreading(), params.supportsMultithreading())
    , _active(false)
    , _identifierQuote('`')
    , _connectionIdOnServer(-1)
    , _connectionParams(params)
    , _characterSet()
    , _isUnicode(false)
    , _useAllDatabases(true)
{
    _keepAliveTimer.setInterval(params.keepAliveTimeoutSeconds() * 1000);
    connect(&_keepAliveTimer, &QTimer::timeout,
            this, &Connection::keepAliveTimeout);
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
    _keepAliveTimer.start();
}

QStringList Connection::allDatabases(bool refresh /*= false */)
{
    if (_allDatabasesCached.first == false || refresh) { // cache is empty or F5
        _allDatabasesCached.second = fetchDatabases();
        _allDatabasesCached.first = true;
    }

    return _allDatabasesCached.second;
}

QStringList Connection::databases(bool refresh /*= false */)
{
    return _useAllDatabases ? allDatabases(refresh) : _databases;
}

void Connection::setDatabases(const QStringList & databases)
{
    _databases = databases;
    setUseAllDatabases(false);
}

void Connection::setUseAllDatabases(bool all)
{
    _useAllDatabases = all;
}

QList<EntityPtr> Connection::getDbEntities(const QString & dbName,
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
        DataBaseEntitiesFetcher * fetcherPtr = createDbEntitiesFetcher();

        std::shared_ptr<DataBaseEntitiesFetcher> fetcher(fetcherPtr);

        QList<EntityPtr> newList = fetcherPtr->run(dbName);
        _databaseEntitiesCache.insert(dbName, newList);

        return newList;
    }
}

bool Connection::deleteAllCachedEntitiesInDatabase(const QString & dbName)
{
    if (_databaseEntitiesCache.contains(dbName)) {
        _databaseEntitiesCache.remove(dbName);
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
        result.reserve(queryPtr->recordCount());
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

QString Connection::getCell(const QString & SQL, const QString & columnName)
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
    if (!query->isEof()) {
        return query->curRow();
    } else {
        return {};
    }
}

QList<QStringList> Connection::getRows(const QString & SQL)
{
    QueryPtr query = getResults(SQL);

    QList<QStringList> result;

    Query * queryPtr = query.get();

    if (queryPtr->recordCount() > 0) {

        result.reserve(queryPtr->recordCount());

        while (queryPtr->isEof() == false) {
            result.append(queryPtr->curRow());
            queryPtr->seekNext();
        }
    }

    return result;
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
        int glueIndex = identifier.indexOf(glue);
        if (glueIndex != -1) { // found
            QString left = identifier.mid(0, glueIndex);
            QString right = identifier.mid(glueIndex+1);
            return quoteIdentifier(left) + glue + quoteIdentifier(right);
        }
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

QString Connection::dequoteIdentifier(const QString & identifier,
                                      QChar glue) const
{
    QString result = identifier;
    if (identifier.length() >= 2
            && identifier[0] == _identifierQuote
            && identifier[identifier.length()-1] == _identifierQuote) {

        result = result.mid(1, result.length()-2);
    }

    if (glue != QChar::Null) {
        result.replace(QString(_identifierQuote+glue+_identifierQuote), glue);
    }

    result.replace(QString(_identifierQuote)+QString(_identifierQuote),
                   QChar(_identifierQuote));

    // TODO: Heidi removes FQuoteChars

    return result;
}

const QStringList Connection::collationList()
{
    if (_collationFetcher == nullptr) {
        _collationFetcher.reset(createCollationFetcher());
    }

    // debug
    if (_collationFetcher == nullptr) {
        return {};
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
    // debug
    if (_tableEnginesFetcher == nullptr) {
        return {};
    }
    return _tableEnginesFetcher->getList();
}

QString Connection::defaultTableEngine()
{
    if (_tableEnginesFetcher == nullptr) {
        _tableEnginesFetcher.reset(createTableEnginesFetcher());
    }
    if (_tableEnginesFetcher == nullptr) {
        return {};
    }
    return _tableEnginesFetcher->defaultEngine();
}

QueryPtr Connection::createQuery()
{
    return std::make_shared<Query>(this);
}

void Connection::emitDatabaseChanged(const QString& newName)
{
    emit databaseChanged(newName);
}

void Connection::stopThread()
{
    _thread.reset();
}

void Connection::keepAliveTimeout()
{
    if (_active) {
        ping(false);
    }
}

void Connection::parseTableStructure(TableEntity * table, bool refresh)
{
    if (table->isNew()) return;
    if (!refresh && table->hasStructure()) {
        return;
    }
    if (_tableStructureParser == nullptr) {
        _tableStructureParser.reset(createTableStructureParser());
    }
    _tableStructureParser->run(table);
}

void Connection::parseViewStructure(ViewEntity * view, bool refresh)
{
    if (!refresh && view->hasStructure()) {
        return;
    }

    if (_viewStructureParser == nullptr) {
        _viewStructureParser.reset(new ViewStructureParser(this));
    }
    _viewStructureParser->run(view);
}

void Connection::parseRoutineStructure(RoutineEntity * routine, bool refresh)
{
    if (!refresh && routine->hasStructure()) {
        return;
    }

    if (_routineStructureParser == nullptr) {
        _routineStructureParser.reset(new RoutineStructureParser(this));
    }
    _routineStructureParser->run(routine);
}

void Connection::parseTriggerStructure(TriggerEntity * trigger, bool refresh)
{
    if (!refresh && trigger->hasStructure()) {
        return;
    }

    TriggerStructureParser parser(this);
    parser.run(trigger);
}

bool Connection::editEntityInDB(EntityInDatabase * entity,
                                EntityInDatabase * newData)
{

    switch (entity->type()) {

    case Entity::Type::Table: {

        Q_ASSERT(entity->type() == newData->type());

        std::unique_ptr<TableEditor> editor(createTableEditor());
        return editor->edit(
                    static_cast<TableEntity *>(entity),
                    static_cast<TableEntity *>(newData));
    }

    case Entity::Type::View: {

        Q_ASSERT(entity->type() == newData->type());

        std::unique_ptr<ViewEditor> editor(createViewEditor());
        return editor->edit(
                    static_cast<ViewEntity *>(entity),
                    static_cast<ViewEntity *>(newData));
    }

    case Entity::Type::Function:
    case Entity::Type::Procedure: {        
        std::unique_ptr<RoutineEditor> editor(createRoutineEditor());
        return editor->edit(
                    static_cast<RoutineEntity *>(entity),
                    static_cast<RoutineEntity *>(newData));
    }

    case Entity::Type::Trigger: {

        Q_ASSERT(entity->type() == newData->type());

        std::unique_ptr<TriggerEditor> editor(createTriggerEditor());
        return editor->edit(
                    static_cast<TriggerEntity *>(entity),
                    static_cast<TriggerEntity *>(newData));
    }

    default:
        Q_ASSERT(false);
        break;
    }

    return false;
}

bool Connection::insertEntityToDB(EntityInDatabase * entity)
{    
    switch (entity->type()) {

    case Entity::Type::Table: {
        std::unique_ptr<TableEditor> editor(createTableEditor());
        return editor->insert(static_cast<TableEntity *>(entity));
    }

    case Entity::Type::View: {
        std::unique_ptr<ViewEditor> editor(createViewEditor());
        return editor->insert(static_cast<ViewEntity *>(entity));
    }

    case Entity::Type::Function:
    case Entity::Type::Procedure: {
        std::unique_ptr<RoutineEditor> editor(createRoutineEditor());
        return editor->insert(static_cast<RoutineEntity *>(entity));
    }

    case Entity::Type::Trigger: {
        std::unique_ptr<TriggerEditor> editor(createTriggerEditor());
        return editor->insert(static_cast<TriggerEntity *>(entity));
    }

    default:
        Q_ASSERT(false);
        break;

    }

    return false;
}

bool Connection::dropEntityInDB(EntityInDatabase * entity)
{
    switch (entity->type()) {

    case Entity::Type::Table: {
        std::unique_ptr<TableEditor> editor(createTableEditor());
        return editor->drop(static_cast<TableEntity *>(entity));
    }

    case Entity::Type::View: {
        std::unique_ptr<ViewEditor> editor(createViewEditor());
        return editor->drop(static_cast<ViewEntity *>(entity));
    }

    case Entity::Type::Function:
    case Entity::Type::Procedure: {
        std::unique_ptr<RoutineEditor> editor(createRoutineEditor());
        return editor->drop(static_cast<RoutineEntity *>(entity));
    }

    case Entity::Type::Trigger: {
        std::unique_ptr<TriggerEditor> editor(createTriggerEditor());
        return editor->drop(static_cast<TriggerEntity *>(entity));
    }

    default:
        Q_ASSERT(false);
        break;

    }

    return false;
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
    std::unique_ptr<DataBaseEditor> editor(createDataBaseEditor());


    editor->create(name, collation);
}

bool Connection::editDatabase(DataBaseEntity * database,
                              const QString & newName,
                              const QString & newCollation)
{
    std::unique_ptr<DataBaseEditor> editor(createDataBaseEditor());

    return editor->edit(database, newName, newCollation);
}

std::shared_ptr<QueryDataEditor> Connection::queryDataEditor()
{
    return std::make_shared<QueryDataEditor>();
}

QString Connection::limitOnePostfix(bool select) const
{
    Q_UNUSED(select)
    return ""; // some dbs dont have it
}

QString Connection::applyLeft(
        const QString & string,
        int length) const
{
    return QString("LEFT(%1, %2)").arg(string, QString::number(length));
}

QDateTime Connection::currentServerTimestamp()
{
    try {
        QString timestamp = getCell("SELECT CURRENT_TIMESTAMP");
        return meow::helpers::parseDateTime(timestamp);
    } catch(meow::db::Exception & ex) {
        Q_UNUSED(ex);
        return {};
    }

}

ConnectionQueryKillerPtr Connection::createQueryKiller() const
{
    return std::make_shared<ConnectionQueryKiller>(
                const_cast<Connection *>(this));
}

ConnectionDataTypes * Connection::dataTypes()
{
    if (_dataTypes == nullptr) {
        _dataTypes.reset(createConnectionDataTypes());
    }
    return _dataTypes.get();
}

ConnectionFeatures * Connection::features()
{
    if (_features == nullptr) {
        _features.reset(createFeatures());
    }
    return _features.get();
}

SessionVariables * Connection::variables()
{
    if (_variables == nullptr) {
        _variables.reset(createVariables());
    }
    return _variables.get();
}

IUserManager * Connection::userManager()
{
    if (_userManager == nullptr) {
        _userManager.reset(createUserManager());
        Q_ASSERT(_userManager != nullptr);
    }
    return _userManager.get();
}

IUserEditor * Connection::userEditor()
{
    if (_userEditor == nullptr) {
        _userEditor.reset(createUserEditor());
        Q_ASSERT(_userEditor != nullptr);
    }
    return _userEditor.get();
}

threads::DbThread * Connection::thread()
{
    if (_thread == nullptr) {
        _thread.reset(new threads::DbThread(this));
    }
    return _thread.get();
}

std::unique_ptr<DbThreadInitializer> Connection::createThreadInitializer() const
{
    return db::createThreadInitializer(connectionParams()->serverType());
}

ViewEditor * Connection::createViewEditor()
{
    return new ViewEditor(this);
}

RoutineEditor * Connection::createRoutineEditor()
{
    return new RoutineEditor(this);
}

TriggerEditor * Connection::createTriggerEditor()
{
    return new TriggerEditor(this);
}

ConnectionFeatures * Connection::createFeatures()
{
    return new ConnectionFeatures(this);
}

ITableStructureParser * Connection::createTableStructureParser()
{
    return new TableStructureParser(this); // default parser
}

} // namespace db
} // namespace meow
