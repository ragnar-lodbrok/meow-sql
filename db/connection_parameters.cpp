#include <QDebug>
#include <QRegularExpression>
#include "db/connection_parameters.h"
#include "db/connection_params_manager.h"
#include "db/mysql_connection.h"

meow::db::ConnectionParameters::ConnectionParameters(ConnectionParamsManager * manager)
   :_networkType(NetworkType::MySQL_TCPIP),
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

bool meow::db::ConnectionParameters::operator==(const meow::db::ConnectionParameters & other)
{
    return _networkType == other._networkType
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
        return ConnectionPtr(connection);
    }
    default:
        qDebug() << "Unimplemented network type";
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


namespace meow {
namespace db {

QString networkTypeName(const NetworkType & networkType, bool longFormat)
{
    if (longFormat) {
        switch (networkType) {
        case NetworkType::MySQL_TCPIP:
            return "MySQL (TCP/IP)";
        default:
            break;
        }
    } else {
        switch (networkType) {
        case NetworkType::MySQL_TCPIP:
            return "MySQL";
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
