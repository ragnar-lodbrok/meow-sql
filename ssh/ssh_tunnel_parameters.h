#ifndef SSH_TUNNEL_PARAMETERS_H
#define SSH_TUNNEL_PARAMETERS_H

#include <QString>

namespace meow {
namespace ssh {

class SSHTunnelParameters
{
public:
    SSHTunnelParameters();
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
