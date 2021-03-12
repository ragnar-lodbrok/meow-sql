#include <QObject> // tr()

#include "mysql_connection.h"
#include "mysql_query.h"
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
#include "ssh/openssh_tunnel.h"
#include "db/entity/view_entity.h"

#include <QDebug>

// https://dev.mysql.com/doc/refman/5.7/en/c-api.html
// https://dev.mysql.com/doc/refman/5.7/en/c-api-building-clients.html
// https://dev.mysql.com/doc/refman/5.7/en/c-api-threaded-clients.html

namespace meow {
namespace db {

MySQLConnection::MySQLConnection(const ConnectionParameters & params)
   : Connection(params)
   , _handle(nullptr)
   , _sshTunnel(nullptr)
{
    _identifierQuote = QLatin1Char('`');
}

MySQLConnection::~MySQLConnection()
{
    meowLogDebugC(this) << "Destroying: " << *connectionParams();
    if (active()) {
        setActive(false);
    }
}

QueryPtr MySQLConnection::createQuery() // override
{
    MySQLQuery * query = new MySQLQuery(this);

    return QueryPtr(query);
}

void MySQLConnection::setActive(bool active) // override
{
    if (active && _handle == nullptr) {
        doBeforeConnect();

        if (connectionParams()->isSSHTunnel()) {

            _sshTunnel.reset(new ssh::OpenSSHTunnel()); // TODO: factory

            //_sshTunnel.reset(new ssh::LibSSHTunnel()); // TODO
            _sshTunnel->connect(*connectionParams()); // throws on error

            connectionParams()->setHostName("127.0.0.1");
            connectionParams()->setPort(
                        connectionParams()->sshTunnel().localPort());
        }

        _handle = mysql_init(nullptr); // TODO: valgrind says it leaks?
        // TODO _handle== NULL

        // TODO: H: SSL, named pipe

        unsigned long clientFlags =
                  CLIENT_LOCAL_FILES
                | CLIENT_INTERACTIVE
                | CLIENT_PROTOCOL_41
                | CLIENT_MULTI_STATEMENTS;

        // TODO: H: flags SSL, COMPRESS
        // TODO: H: MYSQL_PLUGIN_DIR

        QByteArray hostBytes = connectionParams()->hostName().toLatin1();
        QByteArray userBytes = connectionParams()->userName().toLatin1();
        QByteArray pswdBytes = connectionParams()->password().toLatin1();

        const char * host = hostBytes.constData();
        const char * user = userBytes.constData();
        const char * pswd = pswdBytes.constData();
        unsigned int port = connectionParams()->port();

        meowLogDebugC(this) << "Connecting: " << *connectionParams();

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
    const char * errMessage = mysql_error(_handle);

    return QString(errMessage);
}

QString MySQLConnection::fetchCharacterSet() // override
{
    const char * charSet = mysql_character_set_name(_handle);

    QString charsetStr(charSet);

    meowLogDebugC(this) << "Character set: " << charsetStr;

    Connection::setCharacterSet(charsetStr);
    setIsUnicode(charsetStr.startsWith("utf8", Qt::CaseInsensitive));

    return charsetStr;
}

void MySQLConnection::setCharacterSet(const QString & characterSet) // override
{
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
    meowLogDebugC(this) << "Ping";

    if (_handle == nullptr || mysql_ping(_handle) != 0) {
        setActive(false); // TODO: why?
                        // H: Be sure to release some stuff before reconnecting
        if (reconnect) {
            setActive(true);
        }
    }

    // TODO: H: Restart keep-alive timer

    return _active;
}

QueryResults MySQLConnection::query(const QString & SQL,
                                    bool storeResult)
{
    // H: FLockedByThread
    // TODO: need mutex when multithreading

    meowLogCC(Log::Category::SQL, this) << SQL; // TODO: userSQL
    
    QueryResults results;

    ping(true);    
    // TODO: H: FLastQuerySQL

    QByteArray nativeSQL;

    if (isUnicode()) {
        nativeSQL = SQL.toUtf8();
    } else {
        nativeSQL = SQL.toLatin1();
    }

    int queryStatus = mysql_real_query(_handle,
                                       nativeSQL.constData(),
                                       nativeSQL.size());

    if (queryStatus != 0) {
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query failed: " << error;
        throw db::Exception(error);
    }

    // TODO: H: FWarningCount := mysql_warning_count(FHandle);
    _rowsAffected = 0;
    _rowsFound = 0;

    MYSQL_RES * queryResult = nullptr;

    if (storeResult) { 
        queryResult = mysql_store_result(_handle);
    }

    if (queryResult == nullptr
            && mysql_affected_rows(_handle) == (my_ulonglong)-1) {
        // TODO: the doc stands to check mysql_error(), no mysql_affected_rows?
        QString error = getLastError();
        meowLogCC(Log::Category::Error, this) << "Query (store) failed: "
                                              << error;
        throw db::Exception(error);
    }

    // TODO: H: if QueryResult = nil then DetectUSEQuery(SQL);

    while (queryStatus == 0) {
        if (queryResult != nullptr) {
            // Statement returned a result set
            _rowsFound += mysql_num_rows(queryResult);
            if (storeResult) {
                auto result = std::make_shared<MySQLQueryResult>(queryResult);
                results << result;
            } else {
                mysql_free_result(queryResult);
            }
        } else {
            // No result, but probably affected rows
            _rowsAffected += mysql_affected_rows(_handle);
        }

        // more results? -1 = no, >0 = error, 0 = yes (keep looping)
        queryStatus = mysql_next_result(_handle);
        if (queryStatus == 0) {
            queryResult = mysql_store_result(_handle);
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

    meowLogDebugC(this) << "Query rows found/affected: " << _rowsFound
                        << "/" << _rowsAffected;

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

    if (processJokerChars) {
        throw std::runtime_error("not implemented");
    }

    // https://dev.mysql.com/doc/refman/5.7/en/mysql-real-escape-string-quote.html
    // Strictly speaking, MySQL requires only that backslash and the quote
    // character used to quote the string in the query be escaped.

    res.replace(QLatin1Char('\''), QLatin1String("''"));
    res.replace(QLatin1String("\\"), QLatin1String("\\\\"));

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
    if (database == _database) {
        return;
    }

    // TODO: FOnDatabaseChanged
    // TODO: clear database

    query(QString("USE ") + quoteIdentifier(database));
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

} // namespace db
} // namespace meow
