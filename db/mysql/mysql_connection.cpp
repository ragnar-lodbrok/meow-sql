#include "mysql_connection.h"
#include "mysql_connection_query_killer.h"
#include "db/query.h"
#include "mysql_entities_fetcher.h"
#include "mysql_query_data_fetcher.h"
#include "db/entity/table_entity.h"
#include "mysql_table_editor.h"
#include "db/database_editor.h"
#include "mysql_collation_fetcher.h"
#include "mysql_table_engines_fetcher.h"
#include "db/entity/mysql_entity_filter.h"
#include "mysql_query_result.h"
#include "helpers/logger.h"
#include "mysql_database_editor.h"
#include "db/data_type/mysql_connection_data_types.h"
#include "mysql_query_data_editor.h"
#include "mysql_user_manager.h"
#include "mysql_user_editor.h"
#include "ssh/ssh_tunnel_factory.h"
#include "db/entity/view_entity.h"
#include "threads/helpers.h"

#include <QDebug>
#include <QElapsedTimer>
#include <QObject> // tr()

// https://dev.mysql.com/doc/refman/5.7/en/c-api.html
// https://dev.mysql.com/doc/refman/5.7/en/c-api-building-clients.html
// https://dev.mysql.com/doc/c-api/5.7/en/c-api-threaded-clients.html

// Threads logic:
// See: https://dev.mysql.com/doc/c-api/5.7/en/c-api-threaded-clients.html
// 1. Call mysql_library_init() before any other MySQL functions.
// It is not thread-safe, so call it before threads are created, or protect
// the call with a mutex. [Done by MySQLLibraryInitializer]
// 2. Arrange for mysql_thread_init() to be called early in the thread handler
// before calling any MySQL function. [Implemented by MySQLThreadInitializer,
// not called yet]
// Multiple threads cannot send a query to the MySQL server at the same time
// on the same connection. In particular, you must ensure that between calls
// to mysql_real_query() and mysql_store_result() in one thread,
// no other thread uses the same connection. To do this, use a mutex lock
// around your pair of mysql_real_query() and mysql_store_result() calls.
// After mysql_store_result() returns, the lock can be released and other
// threads may query the same connection.

namespace meow {
namespace db {

MySQLConnection::MySQLConnection(const ConnectionParameters & params)
   : Connection(params)
   , _handle(nullptr)
   , _sshTunnel(nullptr)
   , _forkType(MySQLForkType::Original)
{
    _identifierQuote = QLatin1Char('`');
}

MySQLConnection::~MySQLConnection()
{
    meowLogDebugC(this) << "Destroying: " << *connectionParams();
    stopThread();
    if (active()) {
        setActive(false);
    }
}

void MySQLConnection::setActive(bool active) // override
{

    MEOW_ASSERT_MAIN_THREAD

    threads::MutexLocker locker(mutex()); // protects _handle

    if (active && _handle == nullptr) {
        doBeforeConnect();

        if (connectionParams()->isSSHTunnel()) {

            ssh::SSHTunnelFactory sshFactory;
            _sshTunnel = sshFactory.createTunnel();

            _sshTunnel->connect(*connectionParams()); // throws an error

            connectionParams()->setHostName("127.0.0.1");
            connectionParams()->setPort(
                        _sshTunnel->params().localPort());
        }

        _handle = mysql_init(nullptr); // TODO: valgrind says it leaks?
        // TODO _handle== NULL

        // TODO: H: SSL, named pipe

        unsigned long clientFlags =
                  CLIENT_LOCAL_FILES
                | CLIENT_INTERACTIVE
                | CLIENT_PROTOCOL_41
                | CLIENT_MULTI_STATEMENTS;

        // TODO: H: flags SSL
        // TODO: H: MYSQL_PLUGIN_DIR

        QByteArray hostBytes = connectionParams()->hostName().toLatin1();
        QByteArray userBytes = connectionParams()->userName().toLatin1();
        QByteArray pswdBytes = connectionParams()->password().toLatin1();

        const char * host = hostBytes.constData();
        const char * user = userBytes.constData();
        const char * pswd = pswdBytes.constData();
        unsigned int port = connectionParams()->port();

        meowLogDebugC(this) << "Connecting: " << *connectionParams();

        if (connectionParams()->isCompressed()) {
            mysql_options(_handle, MYSQL_OPT_COMPRESS, nullptr);
            mysql_options(_handle, MYSQL_OPT_COMPRESSION_ALGORITHMS, "zlib,zstd");
        }

        MYSQL * connectedHandle = mysql_real_connect(
            _handle,
            host,
            user,
            pswd,
            nullptr, // db
            port,
            nullptr, // unix_socket
            clientFlags
        );

        if (connectedHandle == nullptr) {

            QString error = getLastError();

            meowLogCC(Log::Category::Error, this) << "Connect failed: " << error;
            _sshTunnel.reset();
            // H:
            throw db::Exception(error);
        } else {
            _active = true;
            // Catch late init_connect error by firing mysql_ping(), which detects a broken
            // connection without running into some access violation. See issue #3464.

            if (!ping(false)) {
                meowLogCC(Log::Category::Error, this) << "Connect access violation";

                QString userErrorMessage = QString(QObject::tr(
                    "Connection closed immediately after it was established. " \
                    "This is mostly caused by an \"%1\" server variable which has errors in itself, " \
                    "or your user account does not have the required privileges for it to run.\n\n" \
                    "You may ask someone with SUPER privileges\n" \
                    "* either to fix the \"%1\" variable,\n" \
                    "* or to grant you missing privileges.")).arg(("init_connect"));

                throw db::Exception(userErrorMessage);
            }
        }

        meowLogDebugC(this) << "Connected";

        try {
            setCharacterSet("utf8mb4");
        } catch(meow::db::Exception & ex) {
            meowLogCC(Log::Category::Error, this)
                << "Failed to set utf8mb4 charset: " << ex.message();
            try {
                setCharacterSet("utf8");
            } catch(meow::db::Exception & ex) {
                meowLogCC(Log::Category::Error, this)
                    << "Failed to set utf8 charset: " << ex.message();
            }
        }

        QString curCharSet = fetchCharacterSet();
        Q_UNUSED(curCharSet);

        // H: show status
        _serverVersionString = QString(mysql_get_server_info(_handle));
        _serverVersionInt = static_cast<int>(
            mysql_get_server_version(_handle));
        _forkType = forkTypeFromVersion(_serverVersionString);

        // H: set database

        doAfterConnect();
    } else if (!active && _handle != nullptr) {
        mysql_close(_handle);
        _active = false;
        // H: ClearCache(False);
        _handle = nullptr;
        _sshTunnel.reset();
        meowLogDebugC(this) << "Closed";
    }
}

QString MySQLConnection::getLastError() // override
{
    threads::MutexLocker locker(mutex()); // protects _handle

    const char * errMessage = mysql_error(_handle);

    return QString(errMessage);
}

QString MySQLConnection::fetchCharacterSet() // override
{
    MEOW_ASSERT_MAIN_THREAD

    const char * charSet = mysql_character_set_name(_handle);

    QString charsetStr(charSet);

    meowLogDebugC(this) << "Character set: " << charsetStr;

    Connection::setCharacterSet(charsetStr);
    setIsUnicode(charsetStr.startsWith("utf8", Qt::CaseInsensitive));

    return charsetStr;
}

void MySQLConnection::setCharacterSet(const QString & characterSet) // override
{
    MEOW_ASSERT_MAIN_THREAD

    // H:   FStatementNum := 0

    meowLogDebugC(this) << "Set character set: " << characterSet;

    QByteArray characterSetLatin1 = characterSet.toLatin1();

    int result = mysql_set_character_set(_handle, characterSetLatin1.constData());

    if (result == 0) {
        Connection::setCharacterSet(characterSet);
        setIsUnicode(characterSet.startsWith("utf8", Qt::CaseInsensitive));
    } else {
        throw db::Exception(getLastError());
    }
}

bool MySQLConnection::ping(bool reconnect) // override
{
    MEOW_ASSERT_MAIN_THREAD

    if (mutex()->tryLock() == false) {
        // Don't ping if we are busy in another thread
        return _active;
    }

    //meowLogDebugC(this) << "Ping";

    threads::MutexUnlocker unlocker(mutex()); // protects _handle

    if (_handle == nullptr || mysql_ping(_handle) != 0) {
        setActive(false); // TODO: why?
                        // H: Be sure to release some stuff before reconnecting
        if (reconnect) {
            setActive(true);
        }
    }

    return _active;
}

QueryResults MySQLConnection::query(const QString & SQL,
                                    bool storeResult)
{
    // serializes mysql_real_query() and mysql_store_result()
    // protects _handle
    threads::MutexLocker locker(mutex());

    meowLogCC(Log::Category::SQL, this) << SQL; // TODO: userSQL
    
    QueryResults results;

    if (threads::isCurrentThreadMain()) {
        // ping may change _handle and call UI actions (in future),
        // allow this action in main thread only (temp solution)
        ping(true);
    }
    // TODO: H: FLastQuerySQL

    QByteArray nativeSQL;

    if (isUnicode()) {
        nativeSQL = SQL.toUtf8();
    } else {
        nativeSQL = SQL.toLatin1();
    }

    QElapsedTimer elapsedTimer;

    elapsedTimer.start();
    int queryStatus = mysql_real_query(_handle,
                                       nativeSQL.constData(),
                                       nativeSQL.size());
    results.incExecDuration(std::chrono::milliseconds(elapsedTimer.elapsed()));

    if (queryStatus != 0) {
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query failed: " << error;
        throw db::Exception(error);
    }

    results.setWarningsCount(mysql_warning_count(_handle));

    MYSQL_RES * queryResult = nullptr;

    if (storeResult) { 
        elapsedTimer.start();
        queryResult = mysql_store_result(_handle);
        results.incNetworkDuration(
                std::chrono::milliseconds(elapsedTimer.elapsed()));
    }

    if (queryResult == nullptr
            && mysql_affected_rows(_handle) == (my_ulonglong)-1) {
        // TODO: the doc stands to check mysql_error(), not mysql_affected_rows?
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query (store) failed: "
                                              << error;
        throw db::Exception(error);
    }

    // TODO: H: if QueryResult = nil then DetectUSEQuery(SQL);

    while (queryStatus == 0) {
        if (queryResult != nullptr) {
            // Statement returned a result set
            results.incRowsFound(mysql_num_rows(queryResult));
            if (storeResult) {
                auto result = std::make_shared<MySQLQueryResult>(this);
                result->init(queryResult);
                results << result;
            } else {
                mysql_free_result(queryResult);
            }
        } else {
            // No result, but probably affected rows
            results.incRowsAffected(mysql_affected_rows(_handle));
        }

        // more results? -1 = no, >0 = error, 0 = yes (keep looping)
        elapsedTimer.start();
        queryStatus = mysql_next_result(_handle);
        if (queryStatus == 0) {
            results.incExecDuration(
                        std::chrono::milliseconds(elapsedTimer.elapsed()));
            elapsedTimer.start();
            queryResult = mysql_store_result(_handle);
            results.incNetworkDuration(
                    std::chrono::milliseconds(elapsedTimer.elapsed()));
        } else if (queryStatus > 0) { // err
            // MySQL stops executing a multi-query when an error occurs.
            // So do we here by raising an exception.
            results.clear();
            QString error = getLastError();
            meowLogCC(Log::Category::Error, this) << "Query (next) failed: "
                                                  << error;
            throw db::Exception(error);
        }
    }
    // H:     FResultCount := Length(FLastRawResults);

    meowLogDebugC(this) << "Query rows found/affected: " << results.rowsFound()
                        << "/" << results.rowsAffected();

    return results;
}

QStringList MySQLConnection::fetchDatabases()
{

    try {
        return getColumn("SHOW DATABASES");
    } catch(meow::db::Exception & ex1) {
        Q_UNUSED(ex1);
        try {
            return getColumn("SELECT `SCHEMA_NAME` FROM `information_schema`"
                             ".`SCHEMATA` ORDER BY `SCHEMA_NAME`");
        } catch(meow::db::Exception & ex2) {
            meowLogCC(Log::Category::Error, this)
                << "Database names not available due to missing privileges: "
                << ex2.message();
        }
    }

    return QStringList();
}

QString MySQLConnection::escapeString(const QString & str,
                                      bool processJokerChars,
                                      bool doQuote /*= true*/) const
{

    QString res = str;

    // https://dev.mysql.com/doc/refman/5.7/en/mysql-real-escape-string-quote.html
    // Strictly speaking, MySQL requires only that backslash and the quote
    // character used to quote the string in the query be escaped.

    res.replace(QLatin1Char('\''), QLatin1String("''"));
    res.replace(QLatin1String("\\"), QLatin1String("\\\\"));

    if (processJokerChars) {
        res.replace(QLatin1Char('%'), QLatin1String("\\%"));
        res.replace(QLatin1Char('_'), QLatin1String("\\_"));
    }

    if (doQuote) {
        QLatin1Char singleQuote('\'');
        res = singleQuote + res + singleQuote;
    }

    return res;

    // TODO: NO_BACKSLASH_ESCAPES ?
}

QString MySQLConnection::unescapeString(const QString & str) const
{
    QString res = str;

    res.replace(QLatin1String("\\n"), QString(QChar::LineFeed));
    res.replace(QLatin1String("\\r"), QString(QChar::CarriageReturn));
    res.replace(QLatin1String("\\0"), QString(QChar::Null));
    res.replace(QLatin1String("\\t"), QString(QChar::Tabulation));

    res.replace(QLatin1String("''"), QChar('\''));     // TODO: not tested
    res.replace(QLatin1String("\\\\"), QLatin1String("\\")); // TODO: not tested

    return res;
}

void MySQLConnection::setDatabase(const QString & database) // override
{
    MEOW_ASSERT_MAIN_THREAD

    if (database == _database) {
        return;
    }

    // TODO: FOnDatabaseChanged
    // TODO: clear database

    if (!database.isEmpty()) {
        query(QString("USE ") + quoteIdentifier(database));
    }
    _database = database;
    emitDatabaseChanged(database);

    // TODO: DetectUSEQuery
    // TODO: SetObjectNamesInSelectedDB
}

db::ulonglong MySQLConnection::getRowCount(const TableEntity * table)
{
    const QString SQL = "SHOW TABLE STATUS LIKE " + escapeString(table->name());
    return getCell(SQL, "Rows").toULongLong();
}

QString MySQLConnection::applyQueryLimit(
        const QString & queryType,
        const QString & queryBody,
        db::ulonglong limit,
        db::ulonglong offset) // override
{
    QString res = queryType + " " + queryBody + " LIMIT ";
    if (offset > 0) {
        res += QString::number(offset) + ", ";
    }
    res += QString::number(limit);

    return res;
}

QString MySQLConnection::applyLikeFilter(
            const QList<db::TableColumn *> & columns,
            const QString & value)
{

    QString likeWithValue = " LIKE '%"
            + escapeString(value, true, false)
            + "%'"; // do once, same for all columns

    QStringList conditions;

    for (db::TableColumn * column : columns) {
        QString columnName = quoteIdentifier(column->name());

        db::DataTypeIndex dataType = column->dataType()->index;
        if (dataType == db::DataTypeIndex::Unknown
            || dataType == db::DataTypeIndex::Date
            || dataType == db::DataTypeIndex::DateTime
            || dataType == db::DataTypeIndex::Time
            || dataType == db::DataTypeIndex::Timestamp) {
            columnName = "CAST(" + columnName + " AS CHAR)";
        }

        QString condition = columnName + likeWithValue;

        conditions.push_back(condition);
    }

    return conditions.join(" OR "); // TODO: ESCAPE
}

QueryDataFetcher * MySQLConnection::createQueryDataFetcher() // override
{
    return new MySQLQueryDataFetcher(this);
}

CollationFetcher * MySQLConnection::createCollationFetcher()
{
    return new MySQLCollationFetcher(this);
}

QString MySQLConnection::getCreateCode(const Entity * entity) // override
{
    QString typeStr;
    std::size_t column = 1;

    switch (entity->type()) {

    case Entity::Type::Table:
        typeStr = QString("TABLE");
        column = 1;
        break;

    case Entity::Type::View:
        return getViewCreateCode(static_cast<const ViewEntity*>(entity));

    case Entity::Type::Function:
        typeStr = QString("FUNCTION");
        column = 2;
        break;

    case Entity::Type::Procedure:
        typeStr = QString("PROCEDURE");
        column = 2;
        break;

    case Entity::Type::Trigger:
        typeStr = QString("TRIGGER");
        column = 2;
        break;

    default:
        meowLogDebugC(this) << "Unimplemented type in " << __FUNCTION__;
        Q_ASSERT(false);
        return QString();
    }

    QString SQL = QString("SHOW CREATE %1 %2")
            .arg(typeStr)
            .arg(quotedFullName(entity));

    return getCell(SQL, column);
}

QStringList MySQLConnection::tableRowFormats() const
{
    QStringList formats;
    formats << "DEFAULT" << "DYNAMIC" << "FIXED"
            << "COMPRESSED" << "REDUNDANT" << "COMPACT";
    return formats;
}

std::unique_ptr<EntityFilter> MySQLConnection::entityFilter()
{
    return std::unique_ptr<EntityFilter>(new MySQLEntityFilter(this));
}

std::shared_ptr<QueryDataEditor> MySQLConnection::queryDataEditor()
{
    return std::make_shared<MySQLQueryDataEditor>();
}

DataBaseEntitiesFetcher * MySQLConnection::createDbEntitiesFetcher()
{
    return new MySQLEntitiesFetcher(this);
}

TableEditor * MySQLConnection::createTableEditor()
{
    return new MySQLTableEditor(this);
}

DataBaseEditor * MySQLConnection::createDataBaseEditor()
{
    return new MySQLDataBaseEditor(this);
}

TableEnginesFetcher * MySQLConnection::createTableEnginesFetcher()
{
    return new MySQLTableEnginesFetcher(this);
}

QString MySQLConnection::limitOnePostfix(bool select) const
{
    Q_UNUSED(select); // same for SELECT, UPDATE and DELETE
    return "LIMIT 1";
}

int64_t MySQLConnection::connectionIdOnServer()
{
    MEOW_ASSERT_MAIN_THREAD // TODO: do atomic
    if (_connectionIdOnServer == -1) {
        _connectionIdOnServer = 0; // requesting status to avoid recursion
        _connectionIdOnServer
            = getCell("SELECT CONNECTION_ID()").toLongLong();
    }
    return _connectionIdOnServer;
}

ConnectionQueryKillerPtr MySQLConnection::createQueryKiller() const
{
    return std::make_shared<MySQLConnectionQueryKiller>(
                const_cast<MySQLConnection *>(this));
}

ConnectionDataTypes * MySQLConnection::createConnectionDataTypes()
{
    return new MySQLConnectionDataTypes(this);
}

ConnectionFeatures * MySQLConnection::createFeatures()
{
    return new MySQLConnectionFeatures(this);
}

SessionVariables * MySQLConnection::createVariables()
{
    return new MySQLSessionVariables(this);
}

IUserManager * MySQLConnection::createUserManager()
{
    return new MySQLUserManager(this);
}

IUserEditor * MySQLConnection::createUserEditor()
{
    return new MySQLUserEditor(this);
}

QString MySQLConnection::getViewCreateCode(const ViewEntity * view)
{

    try {

        QString SQL = QString("SHOW CREATE VIEW %1")
                .arg(quotedFullName(view));
        return getCell(SQL, 1);

    } catch (meow::db::Exception & exc) {
        Q_UNUSED(exc); // no permissions for SHOW CREATE VIEW
    }

    QString SQL = QString("SELECT * FROM information_schema.VIEWS WHERE " \
        "TABLE_SCHEMA=%1 AND TABLE_NAME=%2")
            .arg(escapeString(databaseName(view)))
            .arg(escapeString(view->name()));

    QueryPtr schemaResult = getResults(SQL);

    QString definer = schemaResult->curRowColumn("DEFINER");
    QString definition = schemaResult->curRowColumn("VIEW_DEFINITION");
    QString checkOption = schemaResult->curRowColumn("CHECK_OPTION").toUpper();
    QString security = schemaResult->curRowColumn("SECURITY_TYPE").toUpper();

    QString createCode = "CREATE";
    if (!definer.isEmpty()) {
        createCode += QString(" DEFINER=%1")
                .arg(quoteIdentifier(definer, true, '@'));
    }

    if (!security.isEmpty()) {
        createCode += QString(" SQL SECURITY %1")
                .arg(security);
    }

    createCode += QString(" VIEW %1 AS %2")
            .arg(quoteIdentifier(view->name()))
            .arg(definition);

    if (!checkOption.isEmpty() && checkOption != "NONE") {
        createCode += QString(" WITH %1 CHECK OPTION")
                .arg(checkOption);
    }

    return createCode;

}

MySQLForkType MySQLConnection::forkTypeFromVersion(
        const QString & versionString) const
{
    QString version = versionString.toLower();

    if (version.contains("-mariadb")) return MySQLForkType::MariaDB;

    if (version.contains("percona server")) return MySQLForkType::Percona;

    return MySQLForkType::Original;
}

} // namespace db
} // namespace meow
