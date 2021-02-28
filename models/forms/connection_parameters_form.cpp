#include "helpers/logger.h"
#include "connection_parameters_form.h"
#include "db/connection.h"

namespace meow {
namespace models {
namespace forms {

ConnectionParametersForm::ConnectionParametersForm(
        const meow::db::ConnectionParameters & connectionParams)
    : QObject(nullptr)
    , _connectionParams(connectionParams)
{

}

bool ConnectionParametersForm::isEqualTo(const meow::db::ConnectionParameters & connectionParams)
{
    return _connectionParams == connectionParams;
}

void ConnectionParametersForm::setNetworkType(db::NetworkType networkType)
{
    if (_connectionParams.networkType() != networkType) {
        _connectionParams.setNetworkType(networkType);
        _connectionParams.setDefaultValuesForType(networkType);

        emit networkTypeChanged();
        emit changed();
    }
}

void ConnectionParametersForm::setSessionName(const QString &sessionName)
{
    if (_connectionParams.sessionName() != sessionName) {
        _connectionParams.setSessionName(sessionName);
        emit changed();
    }
}

void ConnectionParametersForm::setHostName(const QString &hostName)
{
    if (_connectionParams.hostName() != hostName) {
        _connectionParams.setHostName(hostName);
        emit changed();
    }
}

void ConnectionParametersForm::setFileName(const QString &fileName)
{
    if (_connectionParams.fileName() != fileName) {
        _connectionParams.setFileName(fileName);
        emit changed();
    }

}

void ConnectionParametersForm::setUserName(const QString &userName)
{
    if (_connectionParams.userName() != userName) {
        _connectionParams.setUserName(userName);
        emit changed();
    }
}

void ConnectionParametersForm::setPassword(const QString &password)
{
    if (_connectionParams.password() != password) {
        _connectionParams.setPassword(password);
        emit changed();
    }
}

void ConnectionParametersForm::setDatabases(const QString &databases)
{
    if (_connectionParams.databases() != databases) {
        _connectionParams.setDatabases(databases);
        emit changed();
    }
}

void ConnectionParametersForm::setLoginPrompt(bool loginPrompt)
{
    if (_connectionParams.isLoginPrompt() != loginPrompt) {
        _connectionParams.setLoginPrompt(loginPrompt);
        emit changed();
    }
}

void ConnectionParametersForm::setPort(quint16 port)
{
    if (_connectionParams.port() != port) {
        _connectionParams.setPort(port);
        emit changed();
    }
}

void ConnectionParametersForm::setSSHHost(const QString & host)
{
    if (_connectionParams.sshTunnel().host() != host) {
        _connectionParams.sshTunnel().setHost(host);
        emit changed();
    }
}

void ConnectionParametersForm::setSSHUser(const QString & user)
{
    if (_connectionParams.sshTunnel().user() != user) {
        _connectionParams.sshTunnel().setUser(user);
        emit changed();
    }
}

void ConnectionParametersForm::setSSHPassword(const QString & password)
{
    if (_connectionParams.sshTunnel().password() != password) {
        _connectionParams.sshTunnel().setPassword(password);
        emit changed();
    }
}

void ConnectionParametersForm::setSSHPort(const quint16 port)
{
    if (_connectionParams.sshTunnel().port() != port) {
        _connectionParams.sshTunnel().setPort(port);
        emit changed();
    }
}

void ConnectionParametersForm::setSSHLocalPort(const quint16 localPort)
{
    if (_connectionParams.sshTunnel().localPort() != localPort) {
        _connectionParams.sshTunnel().setLocalPort(localPort);
        emit changed();
    }
}

int ConnectionParametersForm::index() const
{
    return _connectionParams.index();
}

QStringList ConnectionParametersForm::allDatabases()
{
    // TODO: is cashed in connection, rm ?
    if (_allDatabases.first == false) { // cache is empty

        _allDatabases.first = true;

        db::ConnectionPtr connection = _connectionParams.createConnection();

        try {
            connection->setActive(true);
            _allDatabases.second = connection->fetchDatabases();
        } catch(meow::db::Exception & ex) {
            meowLogCC(Log::Category::Error, connection.get())
                << "Failed to fetch db names: " << ex.message();
        }
    }

    return _allDatabases.second;
}

} // namespace forms
} // namespace models
} // namespace meow
