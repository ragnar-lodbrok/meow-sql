#ifndef MODELS_DB_CONNECTIONPARAMETERSFORM_H
#define MODELS_DB_CONNECTIONPARAMETERSFORM_H

#include <QObject>
#include <QString>

#include "db/connection_parameters.h"

namespace meow {
namespace models {
namespace forms {

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

    QStringList allDatabases(); // returns all database names available for this connection

    void setNetworkType(meow::db::NetworkType networkType);
    void setSessionName(const QString &sessionName);
    void setHostName(const QString &hostName);
    void setFileName(const QString &fileName);
    void setUserName(const QString &userName);
    void setPassword(const QString &password);
    void setDatabases(const QString &databases);
    void setLoginPrompt(bool loginPrompt);
    void setPort(quint16 port);

    bool isEqualTo(const meow::db::ConnectionParameters & connectionParams);

    Q_SIGNAL void changed();
    Q_SIGNAL void networkTypeChanged();

private:
    meow::db::ConnectionParameters _connectionParams; // store a copy, so we can edit freely, not affecting real data
    std::pair<bool, QStringList> _allDatabases; // < cached?, data >
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_DB_CONNECTIONPARAMETERSFORM_H
