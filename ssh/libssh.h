#ifndef SSH_LIBSSH_H
#define SSH_LIBSSH_H

#include <libssh/libssh.h>

#include <memory>

namespace meow {
namespace ssh {

class LibSSHChannel;

// TODO: rename to SSHSession?
class LibSSH
{
public:
    LibSSH();

    ~LibSSH();

    int setOption(enum ssh_options_e type, const void* value);

    int connect();

    void disconnect();

    const char* getError();

    enum ssh_known_hosts_e isKnownServer();

    int userAuthPassword(const char* password);

    int userAuthPublicKeyAuto(const char* passphrase);

    std::unique_ptr<LibSSHChannel> newChannel();

    socket_t getFD();

private:
    ssh_session _session;
};

} // namespace ssh
} // namespace meow

#endif
