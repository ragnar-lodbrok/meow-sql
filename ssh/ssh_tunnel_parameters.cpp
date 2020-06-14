#include "ssh_tunnel_parameters.h"

namespace meow {
namespace ssh {

SSHTunnelParameters::SSHTunnelParameters()
    : _port(22)
    , _localPort(0)
{

}

bool SSHTunnelParameters::operator==(const SSHTunnelParameters & other)
{
    return _host == other._host
        && _user == other._user
        && _password == other._password
        && _port == other._port
        && _localPort == other._localPort;
}

} // namespace ssh
} // namespace meow
