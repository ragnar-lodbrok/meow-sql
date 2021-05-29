#include <QRegularExpression>
#include "db/connection_parameters.h"
#include "db/connection_params_manager.h"

#ifdef WITH_MYSQL
#include "db/mysql/mysql_connection.h"
#endif

#ifdef WITH_POSTGRESQL
#include "db/pg/pg_connection.h"
#endif

#ifdef WITH_SQLITE
#include "db/sqlite/sqlite_connection.h"
#endif

#include "helpers/logger.h"

meow::db::ConnectionParameters::ConnectionParameters(
        ConnectionParamsManager * manager)
   :_networkType(NetworkType::MySQL_TCPIP),
    _serverType(ServerType::MySQL),
    _sessionName("Unnamed"),
    _hostName("127.0.0.1"),
    _fileName(""),
    _userName(""),
    _password(""),
    _databases(""),
    _loginPrompt(false),
    _port(0),
    _manager(manager),
    _id(0)
{
    setDefaultValuesForType(NetworkType::MySQL_TCPIP);
}

void meow::db::ConnectionParameters::setNetworkType(NetworkType networkType)
{
    _networkType = networkType;
    switch (networkType) {

#ifdef WITH_MYSQL
    case NetworkType::MySQL_TCPIP:
    case NetworkType::MySQL_SSH_Tunnel:
        _serverType = ServerType::MySQL;
        break;
#endif

#ifdef WITH_POSTGRESQL
    case NetworkType::PG_TCPIP:
    case NetworkType::PG_SSH_Tunnel:
        _serverType = ServerType::PostgreSQL;
        break;
#endif

#ifdef WITH_SQLITE
    case NetworkType::SQLite3_File:
        _serverType = ServerType::SQLite;
        break;
#endif
    default:
        //Q_ASSERT(false);
        _serverType = ServerType::None;
    }
}

bool meow::db::ConnectionParameters::operator==(
        const meow::db::ConnectionParameters & other)
{
    return _networkType == other._networkType
        && _serverType == other._serverType
        && _sessionName == other._sessionName
        && _hostName == other._hostName
        && _fileName == other._fileName
        && _userName == other._userName
        && _password == other._password
        && _databases == other._databases
        && _loginPrompt == other._loginPrompt
        && _port == other._port
        && _sshTunnel == other._sshTunnel;
}

meow::db::ConnectionParameters::operator QString() const
{
#ifdef WITH_SQLITE
    if (_networkType == NetworkType::SQLite3_File) {
        return _fileName;
    }
#endif
    return _userName + "@" + _hostName + ":" +QString::number(_port);
}

void meow::db::ConnectionParameters::setManager(
        ConnectionParamsManager &manager)
{
    _manager = &manager;
}

int meow::db::ConnectionParameters::index() const
{
    if (_manager) {
        return _manager->indexOfParams(*this);
    }

    return -1;
}

meow::db::ConnectionPtr meow::db::ConnectionParameters::createConnection()
{
    switch (_serverType) {

#ifdef WITH_MYSQL
    case ServerType::MySQL: {
        MySQLConnection * connection = new MySQLConnection(*this);
        if (_databases.length() > 0) {
            connection->setDatabases(databaseList());
        } else {
            connection->setUseAllDatabases(true);
        }
        return ConnectionPtr(connection); // TODO: wrap earlier?
    }
#endif

#ifdef WITH_POSTGRESQL
    case ServerType::PostgreSQL: {
        PGConnection * connection = new PGConnection(*this);
        if (_databases.length() > 0) {
            connection->setDatabases(databaseList());
        } else {
            connection->setUseAllDatabases(true);
        }
        return ConnectionPtr(connection);
    }
#endif

#ifdef WITH_SQLITE
    case ServerType::SQLite: {
        SQLiteConnection * connection = new SQLiteConnection(*this);
        connection->setUseAllDatabases(true);
        return ConnectionPtr(connection);
    }
#endif

    default:
        Q_ASSERT(false);
        meowLogC(Log::Category::Error) << "Unimplemented network type";
    }
    return nullptr;
}

void meow::db::ConnectionParameters::setDefaultValuesForType(
        const NetworkType type)
{
    switch (type) {

#ifdef WITH_MYSQL
    case NetworkType::MySQL_TCPIP:
    case NetworkType::MySQL_SSH_Tunnel:

        setNetworkType(type);
        setServerType(ServerType::MySQL);
        if (_hostName.isEmpty()) {
            setHostName("127.0.0.1");
        }
        setUserName("root");
        setPort(3306);

        if (type == NetworkType::MySQL_SSH_Tunnel) {
            _sshTunnel.setLocalPort( port() + 1 );
            _sshTunnel.setPort(22);
            setHostName("127.0.0.1");
        }

        break;
#endif

#ifdef WITH_POSTGRESQL
    case NetworkType::PG_TCPIP:
    case NetworkType::PG_SSH_Tunnel:

        setNetworkType(type);
        setServerType(ServerType::PostgreSQL);
        if (_hostName.isEmpty()) {
            setHostName("127.0.0.1");
        }
        setUserName("postgres");
        setPort(5432);

        if (type == NetworkType::PG_SSH_Tunnel) {
            _sshTunnel.setLocalPort( port() + 1 );
            _sshTunnel.setPort(22);
            setHostName("127.0.0.1");
        }

        break;
#endif

#ifdef WITH_SQLITE
    case NetworkType::SQLite3_File:

        setNetworkType(type);
        setServerType(ServerType::SQLite);
        setHostName(QString());
        setUserName(QString());
        setPassword(QString());
        setPort(0);

        break;
#endif

    default:

        break;
    }
}


QStringList meow::db::ConnectionParameters::databaseList() const
{
    QStringList list = _databases.split(
        QRegularExpression(R"((\s)*;(\s)*)"), // by ";" and rm spaces
        QString::SkipEmptyParts);
    list.removeDuplicates();
    return list;
}

void meow::db::ConnectionParameters::addDatabase(const QString & name,
                                                 bool ignoreIfAll)
{
    if (ignoreIfAll && isAllDatabases()) {
        return;
    }
    if (!_databases.isEmpty()) {
        _databases.append(databasesSeparator);
    }

    _databases.append(name);
}

namespace meow {
namespace db {

QString networkTypeName(const NetworkType & networkType, bool longFormat)
{
    if (longFormat) {
        switch (networkType) {

#ifdef WITH_MYSQL
        case NetworkType::MySQL_TCPIP:
            return "MySQL (TCP/IP)";

        case NetworkType::MySQL_SSH_Tunnel:
            return "MySQL (SSH tunnel)";
#endif

#ifdef WITH_POSTGRESQL
        case NetworkType::PG_TCPIP:
            return QString("PostgreSQL (%1)").arg(QObject::tr("experimental"));

        case NetworkType::PG_SSH_Tunnel:
            return QString("PostgreSQL (SSH tunnel) (%1)")
                    .arg(QObject::tr("experimental"));
#endif

#ifdef WITH_SQLITE
        case NetworkType::SQLite3_File:
            return QString("SQLite 3 (%1)").arg(QObject::tr("experimental"));;
#endif
        default:
            break;
        }
    } else {
        switch (networkType) {
#ifdef WITH_MYSQL
        case NetworkType::MySQL_TCPIP:
        case NetworkType::MySQL_SSH_Tunnel:
            return "MySQL";
#endif

#ifdef WITH_POSTGRESQL
        case NetworkType::PG_TCPIP:
        case NetworkType::PG_SSH_Tunnel:
            return "PostgreSQL";
#endif

#ifdef WITH_SQLITE
        case NetworkType::SQLite3_File:
            return QString("SQLite");
#endif
        default:
            break;
        }
    }

    return  "Unknown";
}

QStringList networkTypeNames()
{
    QStringList names;

    for (const NetworkType type : networkTypes()) {
        names.append( networkTypeName(type) );
    }

    return names;
}

QVector<NetworkType> networkTypes()
{
    QVector<NetworkType> types;
#ifdef WITH_MYSQL
    types << NetworkType::MySQL_TCPIP;
    types << NetworkType::MySQL_SSH_Tunnel;
#endif
#ifdef WITH_POSTGRESQL
    types << NetworkType::PG_TCPIP;
    types << NetworkType::PG_SSH_Tunnel;
#endif
#ifdef WITH_SQLITE
    types << NetworkType::SQLite3_File;
#endif
    return types;
}

} // namespace db
} // namespace meow
