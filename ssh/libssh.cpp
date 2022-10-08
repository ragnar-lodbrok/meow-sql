#include "libssh.h"
#include "libssh_channel.h"

#include <stdexcept>

namespace meow {
namespace ssh {

LibSSH::LibSSH()
    : _session(ssh_new())
{
    if (_session == nullptr) {
        throw std::runtime_error("Error creating libssh tunnel");
    }
}

LibSSH::~LibSSH()
{
    ssh_free(_session);
}

int LibSSH::setOption(enum ssh_options_e type, const void* value)
{
    return ssh_options_set(_session, type, value);
}

int LibSSH::connect()
{
    return ssh_connect(_session);
}

void LibSSH::disconnect()
{
    ssh_disconnect(_session);
}

const char* LibSSH::getError()
{
    return ssh_get_error(_session);
}

enum ssh_known_hosts_e LibSSH::isKnownServer()
{
    return ssh_session_is_known_server(_session);
}

int LibSSH::userAuthPassword(const char* password)
{
    return ssh_userauth_password(_session, nullptr, password);
}

int LibSSH::userAuthPublicKeyAuto(const char* passphrase)
{
    return ssh_userauth_publickey_auto(_session, nullptr, passphrase);
}

std::unique_ptr<LibSSHChannel> LibSSH::newChannel()
{
    return std::unique_ptr<LibSSHChannel>(new LibSSHChannel(&_session));
}

socket_t LibSSH::getFD()
{
    return ssh_get_fd(_session);
}

} // namespace ssh
} // namespace meow
