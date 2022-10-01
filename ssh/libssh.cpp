#include "libssh.h"
#include "libssh_channel.h"

#include <stdexcept>

namespace meow {
namespace ssh {
libssh::libssh()
    : _session(ssh_new())
{
    if (_session == nullptr) {
        throw std::runtime_error("Error creating libssh tunnel");
    }
}

libssh::~libssh()
{
    ssh_free(_session);
}

int libssh::setOption(enum ssh_options_e type, const void* value)
{
    return ssh_options_set(_session, type, value);
}

int libssh::connect()
{
    return ssh_connect(_session);
}

void libssh::disconnect()
{
    ssh_disconnect(_session);
}

const char* libssh::getError()
{
    return ssh_get_error(_session);
}

enum ssh_known_hosts_e libssh::isKnownServer()
{
    return ssh_session_is_known_server(_session);
}

int libssh::userAuthPassword(const char* password)
{
    return ssh_userauth_password(_session, nullptr, password);
}

int libssh::userAuthPublicKeyAuto(const char* passphrase)
{
    return ssh_userauth_publickey_auto(_session, nullptr, passphrase);
}

std::unique_ptr<libssh_channel> libssh::newChannel()
{
    return std::make_unique<libssh_channel>(&_session);
}

socket_t libssh::getFD()
{
    return ssh_get_fd(_session);
}
}
}