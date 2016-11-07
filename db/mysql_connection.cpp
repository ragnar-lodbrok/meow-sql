#include <QDebug>
#include <QObject> // tr()

#include "mysql_connection.h"
#include "mysql_query.h"


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

QueryPtr MySQLConnection::createQuery() // override
{
    MySQLQuery * query = new MySQLQuery(this);

    return QueryPtr(query);
}

void MySQLConnection::setActive(bool active) // override
{
    if (active && _handle == nullptr) {
        doBeforeConnect();

        _handle = mysql_init(nullptr);
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
        // H: mysql_get_server_info
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
    // TODO: H: FLockedByThread
    
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

MySQLResult createSharedMySQLResultFromNative(MYSQL_RES * nativeMySQLRes)
{
    return std::shared_ptr<MYSQL_RES> (nativeMySQLRes, [](MYSQL_RES * nativeMySQLRes) {
        if (nativeMySQLRes) {
           mysql_free_result(nativeMySQLRes);
           //qDebug() << "mysql_free_result: " << nativeMySQLRes;
        }
    });
}

} // namespace db
} // namespace meow
