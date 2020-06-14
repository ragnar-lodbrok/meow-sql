#ifndef SSH_TUNNEL_PARAMETERS_H
#define SSH_TUNNEL_PARAMETERS_H

#include <QString>

namespace meow {
namespace ssh {

class SSHTunnelParameters
{
public:
    SSHTunnelParameters();

    void setHost(const QString & host) { _host = host; }
    void setUser(const QString & user) { _user = user; }
    void setPassword(const QString & password) { _password = password; }
    void setPort(const quint16 port) {_port = port; }
    void setLocalPort(const quint16 port) {_localPort = port; }

    QString host() const { return _host; }
    QString user() const { return _user; }
    QString password() const { return _password; }
    quint16 port() const { return _port; }
    quint16 localPort() const { return _localPort; }

    bool operator==(const SSHTunnelParameters & other);

private:
    QString _host;
    QString _user;
    QString _password;

    quint16 _port;
    quint16 _localPort;
};

} // namespace ssh
} // namespace meow

#endif // SSH_TUNNEL_PARAMETERS_H
