#ifndef DB_CONNECTION_PARAMETERS_H
#define DB_CONNECTION_PARAMETERS_H

#include <QString>
#include <QStringList>

namespace meow {
namespace db {

class ConnectionParamsManager;

enum class NetworkType {
    MySQL_TCPIP,
    // TODO: add the rest
    COUNT
};

QString networkTypeName(const NetworkType & networkType, bool longFormat = true);
QStringList networkTypeNames();

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
    bool isLoginPrompt() const { return _loginPrompt; }
    quint16 port() const { return _port; }

    // setters
    void setNetworkType(NetworkType networkType) { _networkType = networkType; }
    void setSessionName(const QString &sessionName) { _sessionName = sessionName; }
    void setHostName(const QString &hostName) { _hostName = hostName; }
    void setUserName(const QString &userName) { _userName = userName; }
    void setPassword(const QString &password) { _password = password; }
    void setDatabases(const QString &databases) { _databases = databases; }
    void setLoginPrompt(bool loginPrompt) { _loginPrompt = loginPrompt; }
    void setPort(qint16 port) { _port = port; }
    void setManager(ConnectionParamsManager &manager);

    bool operator==(const ConnectionParameters & other);
    operator QString() const;
    int index() const;

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
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_PARAMETERS_H
