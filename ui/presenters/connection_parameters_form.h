#ifndef MODELS_DB_CONNECTIONPARAMETERSFORM_H
#define MODELS_DB_CONNECTIONPARAMETERSFORM_H

#include <QObject>
#include <QString>

#include "db/connection_parameters.h"

namespace meow {
namespace ui {
namespace presenters {

class ConnectionParametersForm : public QObject
{
    Q_OBJECT

public:
    explicit ConnectionParametersForm(
        const meow::db::ConnectionParameters & connectionParams
            = meow::db::ConnectionParameters()
    );
    virtual ~ConnectionParametersForm() {}

    meow::db::NetworkType networkType() const {
        return _connectionParams.networkType();
    }
    QString sessionName() const { return _connectionParams.sessionName(); }
    QString hostName() const { return _connectionParams.hostName(); }
    QString fileName() const { return _connectionParams.fileName(); }
    QString userName() const { return _connectionParams.userName(); }
    QString password() const { return _connectionParams.password(); }
    QString databases() const { return _connectionParams.databases(); }
    bool isLoginPrompt() const { return _connectionParams.isLoginPrompt(); }
    bool isCompressed() const { return _connectionParams.isCompressed(); }
    quint16 port() const { return _connectionParams.port(); }
    int index() const;
    const meow::db::ConnectionParameters & connectionParams() const {
        return _connectionParams;
    }

    bool isFilebased() const {
        return _connectionParams.isFilebased();
    }
    bool supportsAuth() const {
        return _connectionParams.supportsAuth();
    }
    bool supportsMultipleDatabases() const {
        return _connectionParams.supportsMultipleDatabases();
    }
    bool supportsSSHPassword() const;

    bool isSSHTunnel() const {
        return _connectionParams.isSSHTunnel();
    }

    QString sshHost() const { return _connectionParams.sshTunnel().host(); }
    QString sshUser() const { return _connectionParams.sshTunnel().user(); }
    QString sshPassword() const {
        return _connectionParams.sshTunnel().password(); }
    quint16 sshPort() const { return _connectionParams.sshTunnel().port(); }
    quint16 sshLocalPort() const {
        return _connectionParams.sshTunnel().localPort(); }


    QStringList allDatabases(); // all database names for this connection

    void setNetworkType(meow::db::NetworkType networkType);
    void setSessionName(const QString &sessionName);
    void setHostName(const QString &hostName);
    void setFileName(const QString &fileName);
    void setUserName(const QString &userName);
    void setPassword(const QString &password);
    void setDatabases(const QString &databases);
    void setLoginPrompt(bool loginPrompt);
    void setCompressed(bool compressed);
    void setPort(quint16 port);

    void setSSHHost(const QString & host);
    void setSSHUser(const QString & user);
    void setSSHPassword(const QString & password);
    void setSSHPort(const quint16 port);
    void setSSHLocalPort(const quint16 localPort);

    bool isEqualTo(const meow::db::ConnectionParameters & connectionParams);

    Q_SIGNAL void changed();
    Q_SIGNAL void networkTypeChanged();

private:
    // store a copy, so we can edit freely, not affecting real data:
    meow::db::ConnectionParameters _connectionParams;
    std::pair<bool, QStringList> _allDatabases; // < cached?, data >
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MODELS_DB_CONNECTIONPARAMETERSFORM_H
