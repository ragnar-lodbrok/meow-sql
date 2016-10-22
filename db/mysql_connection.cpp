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

Query * MySQLConnection::createQuery() const // override
{
    return NULL;
    //return new MySQLQuery(this);
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

/*
function TMySQLConnection.GetAllDatabases: TStringList;
begin
  Result := inherited;
  if not Assigned(Result) then begin
    try
      FAllDatabases := GetCol('SHOW DATABASES');
    except on E:EDatabaseError do
      try
        FAllDatabases := GetCol('SELECT '+QuoteIdent('SCHEMA_NAME')+' FROM '
        +QuoteIdent('information_schema')+'.'+QuoteIdent('SCHEMATA')+' ORDER BY '+QuoteIdent('SCHEMA_NAME'));
      except
        on E:EDatabaseError do begin
          FAllDatabases := TStringList.Create;
          Log(lcError, f_('Database names not available due to missing privileges for user %s.', [CurrentUserHostCombination]));
        end;
      end;
    end;
    Result := FAllDatabases;
  end;
end;
*/

QStringList MySQLConnection::fetchDatabases() // override
{
    return QStringList();
}

} // namespace db
} // namespace meow
