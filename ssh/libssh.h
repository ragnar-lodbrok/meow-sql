#pragma once

#include <libssh/libssh.h>

#include <memory>

namespace meow {
namespace ssh {
class libssh_channel;

class libssh
{
public:
    libssh();

    ~libssh();

    int setOption(enum ssh_options_e type, const void* value);

    int connect();

    void disconnect();

    const char* getError();

    enum ssh_known_hosts_e isKnownServer();

    int userAuthPassword(const char* password);

    int userAuthPublicKeyAuto(const char* passphrase);

    std::unique_ptr<libssh_channel> newChannel();

    socket_t getFD();

private:
    ssh_session _session;
};
}
}
