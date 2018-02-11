#include <QDebug>
#include <QObject> // tr()

#include "mysql_connection.h"
#include "mysql_query.h"
#include "entity/mysql_entities_fetcher.h"
#include "mysql_query_data_fetcher.h"
#include "db/entity/table_entity.h"
#include "mysql_table_editor.h"
#include "mysql_collation_fetcher.h"

// https://dev.mysql.com/doc/refman/5.7/en/c-api.html
// https://dev.mysql.com/doc/refman/5.7/en/c-api-building-clients.html
// https://dev.mysql.com/doc/refman/5.7/en/c-api-threaded-clients.html

namespace meow {
namespace db {

MySQLConnection::MySQLConnection(const ConnectionParameters & params)
    :Connection(params),
     _handle(nullptr)
{

}

MySQLConnection::~MySQLConnection()
{
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

        _handle = mysql_init(nullptr); // TODO: valgrind says it leaks?
        // TODO _handle== NULL

        // TODO: H: SSL, named pipe, SSH

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

        qDebug() << "[MySQLConnection] " << "Connecting: " << *connectionParams();

        MYSQL * connectedHandle = mysql_real_connect(
            _handle,
            host,
            user,
            pswd,
            NULL, // db
            port,
            NULL, // unix_socket
            clientFlags
        );

        if (connectedHandle == nullptr) {

            QString error = getLastError();

            qDebug() << "[MySQLConnection] " << "Connect failed: " << error;
            // H:
            throw db::Exception(error);
        } else {
            _active = true;
            // Catch late init_connect error by firing mysql_ping(), which detects a broken
            // connection without running into some access violation. See issue #3464.

            if (!ping(false)) {
                qDebug() << "[MySQLConnection] " << "Connect access violation";

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

        qDebug() << "[MySQLConnection] " << "Connected";

        try {
            setCharacterSet("utf8mb4");
        } catch(meow::db::Exception & ex) {
            qDebug() << "[MySQLConnection] " << "Failed to set utf8mb4 charset: " << ex.message();
            try {
                setCharacterSet("utf8");
            } catch(meow::db::Exception & ex) {
                qDebug() << "[MySQLConnection] " << "Failed to set utf8 charset: " << ex.message();
            }
        }

        QString curCharSet = fetchCharacterSet();

        // H: show status
        _serverVersionString = QString(mysql_get_server_info(_handle));
        _serverVersionInt = mysql_get_server_version(_handle);

        // H: set database

        doAfterConnect();
    } else if (!active && _handle != nullptr) {
        mysql_close(_handle);
        _active = false;
        // H: ClearCache(False);
        _handle = nullptr;
        qDebug() << "[MySQLConnection] " << "Closed";
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

    qDebug() << "[MySQLConnection] " << "Character set: " << charsetStr;

    Connection::setCharacterSet(charsetStr);
    setIsUnicode(charsetStr.startsWith("utf8", Qt::CaseInsensitive));

    return charsetStr;
}

void MySQLConnection::setCharacterSet(const QString & characterSet) // override
{
    // H:   FStatementNum := 0

    qDebug() << "[MySQLConnection] " << "Set character set: " << characterSet;

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
    qDebug() << "[MySQLConnection] " << "Ping";

    if (_handle == nullptr || mysql_ping(_handle) != 0) {
        setActive(false); // TODO: why? H: Be sure to release some stuff before reconnecting
        if (reconnect) {
            setActive(true);
        }
    }

    // TODO: H: Restart keep-alive timer

    return _active;
}

void MySQLConnection::query(const QString & SQL, bool storeResult) // override
{
    // H: FLockedByThread
    // TODO: need mutex when multithreading

    qDebug() << "[MySQLConnection] " << "Query: " << SQL;
    
    ping(true);    
    // TODO: H: FLastQuerySQL

    QByteArray nativeSQL;

    if (isUnicode()) {
        nativeSQL = SQL.toUtf8();
    } else {
        nativeSQL = SQL.toLatin1();
    }

    _statementNum = 1;
    int queryStatus = mysql_real_query(_handle, nativeSQL.constData(), nativeSQL.size());

    if (queryStatus != 0) {
        QString error = getLastError();
        qDebug() << "[MySQLConnection] " << "Query failed: " << error;
        throw db::Exception(error);
    }

    // TODO: H: FWarningCount := mysql_warning_count(FHandle);
    _rowsAffected = 0;
    _rowsFound = 0;
    _lastRawResults.clear();

    MYSQL_RES * queryResult = mysql_store_result(_handle);

    if (queryResult == nullptr && mysql_affected_rows(_handle) == (my_ulonglong)-1) {
        // TODO: the doc stands to check mysql_error(), no mysql_affected_rows ?
        QString error = getLastError();
        qDebug() << "[MySQLConnection] " << "Query (store) failed: " << error;
        throw db::Exception(error);
    }

    // TODO: H: if QueryResult = nil then DetectUSEQuery(SQL);

    while (queryStatus == 0) {
        if (queryResult != nullptr) {
            // Statement returned a result set
            _rowsFound += mysql_num_rows(queryResult);
            if (storeResult) {
                _lastRawResults.push_back(
                   createSharedMySQLResultFromNative(queryResult)
                ); // smells, but just copy now till understand
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
            ++_statementNum; // H does it above if
            queryResult = mysql_store_result(_handle);
        } else if (queryStatus > 0) { // err
            // MySQL stops executing a multi-query when an error occurs. So do we here by raising an exception.
            _lastRawResults.clear();
            QString error = getLastError();
            qDebug() << "[MySQLConnection] " << "Query (next) failed: " << error;
            throw db::Exception(error);
        }
    }
    // H:     FResultCount := Length(FLastRawResults);

    qDebug() << "[MySQLConnection] " << "Query rows found/affected: " << _rowsFound << "/" << _rowsAffected;

    // TODO: we should simply return raw results as new type (call free in dtor)?
}

QStringList MySQLConnection::fetchDatabases() // override
{

    try {
        return getColumn("SHOW DATABASES");
    } catch(meow::db::Exception & ex1) {
        Q_UNUSED(ex1);
        try {
            return getColumn("SELECT `SCHEMA_NAME` FROM `information_schema`.`SCHEMATA` ORDER BY `SCHEMA_NAME`");
        } catch(meow::db::Exception & ex2) {
            qDebug() << "[MySQLConnection] " << "Database names not available due to missing privileges: " << ex2.message();
        }
    }

    return QStringList();
}


std::size_t MySQLConnection::lastResultsCount() const // override
{
    // listening: Killswitch Engage - The End Of Heartache
    return _lastRawResults.size();
}

MySQLResult MySQLConnection::lastRawResultAt(std::size_t index) const
{
    return _lastRawResults.at(index);
}

QString MySQLConnection::escapeString(const QString & str,
                                      bool processJokerChars,
                                      bool doQuote /*= true*/) const // override
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

db::ulonglong MySQLConnection::getRowCount(const TableEntity * table) // override
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
    int column = 1;

    switch (entity->type()) {
    case Entity::Type::Table:
        typeStr = QString("TABLE");
        column = 1;
        break;
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
        qDebug() << "Unimplemented type in " << __FUNCTION__;
        return QString();
        break;
    }

    QString SQL = QString("SHOW CREATE %1 %2")
            .arg(typeStr)
            .arg(quotedFullName(entity));

    return getCell(SQL, column);
}

MySQLResult createSharedMySQLResultFromNative(MYSQL_RES * nativeMySQLRes)
{
    return std::shared_ptr<MYSQL_RES> (nativeMySQLRes,
        [](MYSQL_RES * nativeMySQLRes) {
            if (nativeMySQLRes) {
                mysql_free_result(nativeMySQLRes);
            }
        }
    );
}

DataBaseEntitiesFetcher * MySQLConnection::createDbEntitiesFetcher() // override
{
    return new MySQLEntitiesFetcher(this);
}

TableEditor * MySQLConnection::createTableEditor()
{
    return new MySQLTableEditor(this);
}

} // namespace db
} // namespace meow
