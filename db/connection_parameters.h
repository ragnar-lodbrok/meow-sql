#ifndef DB_CONNECTION_PARAMETERS_H
#define DB_CONNECTION_PARAMETERS_H

#include <memory>
#include <QString>
#include <QStringList>
#include <QFileInfo>

namespace meow {
namespace db {

class ConnectionParamsManager;
class Connection;

enum class NetworkType { // TODO: move to separate file ?
    MySQL_TCPIP = 0, // keep numbers or rewrite saving changes
    PG_TCPIP = 1,
    SQLite3_File = 2,
    MySQL_SSH_Tunnel = 3,
    // TODO: add the rest
    COUNT = 4
};

enum class ServerType { // TODO: move to separate file ?
    None,
    MySQL,
    PostgreSQL,
    SQLite
    // TODO maria etc
};

QString networkTypeName(const NetworkType & networkType, bool longFormat = true);
QStringList networkTypeNames();

typedef std::shared_ptr<Connection> ConnectionPtr;

const char databasesSeparator = ';';

// Intent: holds single db connection param
class ConnectionParameters
{
public:
    explicit ConnectionParameters(ConnectionParamsManager * manager = nullptr);

    // getters
    NetworkType networkType() const { return _networkType; }
    ServerType serverType() const { return _serverType; }
    QString sessionName() const { return _sessionName; }
    QString hostName() const { return _hostName; }
    QString fileName() const { return _fileName; }
    QString userName() const { return _userName; }
    QString password() const { return _password; }
    bool isAllDatabases() const { return _databases.isEmpty(); }
    QString databases() const { return _databases; }   
    QStringList databaseList() const;
    bool isLoginPrompt() const { return _loginPrompt; }
    quint16 port() const { return _port; }
    bool fullTableStatus() const { return true; }
    unsigned id() const { return _id; }

    // setters
    void setNetworkType(NetworkType networkType);
    void setServerType(ServerType serverType) { _serverType = serverType; }
    void setSessionName(const QString &session) { _sessionName = session; }
    void setHostName(const QString &hostName) { _hostName = hostName; }
    void setFileName(const QString &fileName) { _fileName = fileName; }
    void setUserName(const QString &userName) { _userName = userName; }
    void setPassword(const QString &password) { _password = password; }
    void setDatabases(const QString &databases) { _databases = databases; }
    void addDatabase(const QString & name, bool ignoreIfAll = false);
    void setLoginPrompt(bool loginPrompt) { _loginPrompt = loginPrompt; }
    void setPort(quint16 port) { _port = port; }
    void setManager(ConnectionParamsManager &manager);
    void setId(unsigned id) { _id = id; }

    bool isFilebased() const {
        return _networkType == NetworkType::SQLite3_File;
    }
    bool supportsAuth() const {
        return _networkType != NetworkType::SQLite3_File;
    }
    bool supportsMultipleDatabases() const {
        return _networkType != NetworkType::SQLite3_File;
    }

    bool isSSHTunnel() const {
        return _networkType == NetworkType::MySQL_SSH_Tunnel;
    }

    QString fileNameShort() const {
        QFileInfo fileInfo(_fileName);
        return fileInfo.fileName();
    }

    bool operator==(const ConnectionParameters & other);
    operator QString() const;
    int index() const;

    ConnectionPtr createConnection();

    void setDefaultValuesForType(const NetworkType type);

private:
    NetworkType _networkType;
    ServerType _serverType;
    QString _sessionName;
    QString _hostName;
    QString _fileName;
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
