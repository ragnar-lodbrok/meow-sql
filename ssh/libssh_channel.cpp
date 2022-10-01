#include "libssh_channel.h"

#include <stdexcept>

namespace meow::ssh {
meow::ssh::libssh_channel::libssh_channel(const ssh_session* session)
    : _channel(ssh_channel_new(*session))
{
    if (_channel == nullptr) {
        throw new std::runtime_error("Failed to initialise forwarding channel");
    }
}

void libssh_channel::setBlocking(int blocking)
{
    ssh_channel_set_blocking(_channel, blocking);
}

libssh_channel::~libssh_channel()
{
    ssh_channel_free(_channel);
}

int libssh_channel::openForward(const char* remotehost, int remoteport, const char* sourcehost,
                                int localport)
{
    return ssh_channel_open_forward(_channel, remotehost, remoteport, sourcehost, localport);
}

int libssh_channel::write(const void* data, uint32_t len)
{
    return ssh_channel_write(_channel, data, len);
}

int libssh_channel::readNonBlocking(void* dest, uint32_t count, int is_stderr)
{
    return ssh_channel_read_nonblocking(_channel, dest, count, is_stderr);
}

int libssh_channel::poll(int is_stderr)
{
    return ssh_channel_poll(_channel, is_stderr);
}

int libssh_channel::read(void* dest, uint32_t count, int is_stderr)
{
    return ssh_channel_read(_channel, dest, count, is_stderr);
}

int libssh_channel::pollForMs(int is_stderr, uint32_t ms)
{
    return ssh_channel_poll_timeout(_channel, ms, is_stderr);
}
}

