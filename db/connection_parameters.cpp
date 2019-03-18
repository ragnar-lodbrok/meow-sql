#include <QRegularExpression>
#include "db/connection_parameters.h"
#include "db/connection_params_manager.h"
#include "db/mysql_connection.h"
#include "db/pg/pg_connection.h"
#include "helpers/logger.h"

meow::db::ConnectionParameters::ConnectionParameters(ConnectionParamsManager * manager)
   :_networkType(NetworkType::MySQL_TCPIP),
    _serverType(ServerType::MySQL),
    _sessionName("Unnamed"),
    _hostName("127.0.0.1"),
    _userName("root"),
    _password(""),
    _databases(""),
    _loginPrompt(false),
    _port(3306),
    _manager(manager),
    _id(0)
{

}

void meow::db::ConnectionParameters::setNetworkType(NetworkType networkType)
{
    _networkType = networkType;
    switch (networkType) {
    case NetworkType::MySQL_TCPIP:
        _serverType = ServerType::MySQL;
        break;
    case NetworkType::PG_TCPIP:
        _serverType = ServerType::PostgreSQL;
        break;
    default:
        Q_ASSERT(false);
        _serverType = ServerType::None;
    }
}

bool meow::db::ConnectionParameters::operator==(const meow::db::ConnectionParameters & other)
{
    return _networkType == other._networkType
        && _serverType == other._serverType
        && _sessionName == other._sessionName
        && _hostName == other._hostName
        && _userName == other._userName
        && _password == other._password
        && _databases == other._databases
        && _loginPrompt == other._loginPrompt
        && _port == other._port;
}

meow::db::ConnectionParameters::operator QString() const
{
    return _userName + "@" + _hostName + ":" +QString::number(_port);
}

void meow::db::ConnectionParameters::setManager(ConnectionParamsManager &manager)
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
    switch (_networkType) {

    case NetworkType::MySQL_TCPIP: {
        MySQLConnection * connection = new MySQLConnection(*this);
        if (_databases.length() > 0) {
            connection->setAllDatabases(databaseList());
        } else {
            connection->setUseAllDatabases();
        }
        return ConnectionPtr(connection); // TODO: wrap earlier?
    }

    case NetworkType::PG_TCPIP: {
        PGConnection * connection = new PGConnection(*this);
        if (_databases.length() > 0) {
            connection->setAllDatabases(databaseList());
        } else {
            connection->setUseAllDatabases();
        }
        return ConnectionPtr(connection);
    }
    default:
        meowLogC(Log::Category::Error) << "Unimplemented network type";
    }
    return nullptr;
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
        case NetworkType::MySQL_TCPIP:
            return "MySQL (TCP/IP)";

        case NetworkType::PG_TCPIP:
            return QString("PostgreSQL (%1)").arg(QObject::tr("experimental"));

        default:
            break;
        }
    } else {
        switch (networkType) {
        case NetworkType::MySQL_TCPIP:
            return "MySQL";

        case NetworkType::PG_TCPIP:
            return "PostgreSQL";

        default:
            break;
        }
    }

    return  "Unknown";
}

QStringList networkTypeNames()
{
    QStringList names;

    using IntType = int; //typename std::underlying_type<NetworkType>::type;

    IntType start = 0; //static_cast<IntType>(NetworkType::MySQL_TCPIP);
    IntType end = static_cast<IntType>(NetworkType::COUNT);

    for ( IntType i = start; i < end; ++i ) {
        names.append( networkTypeName( static_cast<NetworkType>(i) ) );
    }

    return names;
}

} // namespace db
} // namespace meow
