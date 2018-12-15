#ifndef DB_CONNECTION_PARAMETERS_H
#define DB_CONNECTION_PARAMETERS_H

#include <memory>
#include <QString>
#include <QStringList>

namespace meow {
namespace db {

class ConnectionParamsManager;
class Connection;

enum class NetworkType {
    MySQL_TCPIP,
    // TODO: add the rest
    COUNT
};

QString networkTypeName(const NetworkType & networkType, bool longFormat = true);
QStringList networkTypeNames();

typedef std::shared_ptr<Connection> ConnectionPtr;

const char databasesSeparator = ';';

// Intent: holds single db connection param
class ConnectionParameters
{
public:
    ConnectionParameters(ConnectionParamsManager * manager = nullptr);

    // getters
    NetworkType networkType() const { return _networkType; }
    QString sessionName() const { return _sessionName; }
    QString hostName() const { return _hostName; }
    QString userName() const { return _userName; }
    QString password() const { return _password; }
    QString databases() const { return _databases; }   
    QStringList databaseList() const;
    bool isLoginPrompt() const { return _loginPrompt; }
    quint16 port() const { return _port; }
    bool fullTableStatus() const { return true; }
    unsigned id() const { return _id; }

    // setters
    void setNetworkType(NetworkType networkType) { _networkType = networkType; }
    void setSessionName(const QString &sessionName) { _sessionName = sessionName; }
    void setHostName(const QString &hostName) { _hostName = hostName; }
    void setUserName(const QString &userName) { _userName = userName; }
    void setPassword(const QString &password) { _password = password; }
    void setDatabases(const QString &databases) { _databases = databases; }
    void setLoginPrompt(bool loginPrompt) { _loginPrompt = loginPrompt; }
    void setPort(quint16 port) { _port = port; }
    void setManager(ConnectionParamsManager &manager);
    void setId(unsigned id) { _id = id; }

    bool operator==(const ConnectionParameters & other);
    operator QString() const;
    int index() const;

    ConnectionPtr createConnection();

private:
    NetworkType _networkType;
    QString _sessionName;
    QString _hostName;
    QString _userName;
    QString _password;
    QString _databases;
    bool _loginPrompt;
    quint16 _port;
    ConnectionParamsManager * _manager;
    unsigned _id;
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_PARAMETERS_H
