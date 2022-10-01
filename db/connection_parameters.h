#ifndef DB_CONNECTION_PARAMETERS_H
#define DB_CONNECTION_PARAMETERS_H

#include <memory>
#include <QString>
#include <QStringList>
#include <QFileInfo>
#include "ssh/ssh_tunnel_parameters.h"
#include "common.h"

namespace meow {
namespace db {

class ConnectionParamsManager;
class Connection;

enum class NetworkType { // TODO: move to separate file ?
    MySQL_TCPIP = 0, // keep numbers or rewrite saving changes
    PG_TCPIP = 1,
    SQLite3_File = 2,
    MySQL_SSH_Tunnel = 3,
    PG_SSH_Tunnel = 4,
    // TODO: add the rest
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
QVector<NetworkType> networkTypes();

using ConnectionPtr = std::shared_ptr<Connection>;
using SSHTunnelParametersPtr = std::shared_ptr<ssh::SSHTunnelParameters>;

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
    bool isCompressed() const { return _isCompressed; }
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
    void setCompressed(bool compressed) { _isCompressed = compressed; }
    void setPort(quint16 port) { _port = port; }
    void setManager(ConnectionParamsManager &manager);
    void setId(unsigned id) { _id = id; }

    bool isFilebased() const {
#ifdef WITH_SQLITE
        if (_networkType == NetworkType::SQLite3_File) {
            return true;
        }
#endif
        return false;
    }
    bool supportsAuth() const {
#ifdef WITH_SQLITE
        if (_networkType == NetworkType::SQLite3_File) {
            return false;
        }
#endif
        return true;
    }
    bool supportsMultipleDatabases() const {
#ifdef WITH_SQLITE
        if (_networkType == NetworkType::SQLite3_File) {
            return false;
        }
#endif
        return true;
    }

    bool supportsCompressionOption() const {
#ifdef WITH_MYSQL
        if (_serverType == ServerType::MySQL) {
            return true;
        }
#endif
        return false;
    }

    bool isSSHTunnel() const {
#ifdef WITH_MYSQL
        if (_networkType == NetworkType::MySQL_SSH_Tunnel) {
            return true;
        }
#endif
#ifdef WITH_POSTGRESQL
        if (_networkType == NetworkType::PG_SSH_Tunnel) {
            return true;
        }
#endif
        return false;
    }

    bool supportsMultithreading() const {
#ifdef MEOW_NO_MULTHITHREADING
        return false;
#endif
#ifdef WITH_MYSQL
        if (_serverType == ServerType::MySQL) {
            return true;
        }
#endif
        return false; // not implemented for other or not supported
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

    ssh::SSHTunnelParameters & sshTunnel() {
        return _sshTunnel;
    }

    const ssh::SSHTunnelParameters & sshTunnel() const {
        return _sshTunnel;
    }

    int keepAliveTimeoutSeconds() const {
        return meow::db::DEFAULT_KEEP_ALIVE_TIMEOUT;
    }

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
    bool _isCompressed;
    quint16 _port;
    ConnectionParamsManager * _manager;
    unsigned _id;
    ssh::SSHTunnelParameters _sshTunnel;
};


} // namespace db
} // namespace meow

#endif // DB_CONNECTION_PARAMETERS_H
