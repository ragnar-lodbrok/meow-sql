#ifndef SSH_SOCKETS_SOCKET_RECEIVER_INTERFACE_H
#define SSH_SOCKETS_SOCKET_RECEIVER_INTERFACE_H

#include <memory>

namespace meow {

class Connection;

class IConnectionReceiver;

class ISocketReceiver
{
public:
    virtual ~ISocketReceiver() {}
    virtual std::shared_ptr<IConnectionReceiver> onNewConnection(
            const std::shared_ptr<Connection>& connection) = 0;
    virtual void onConnectionClosed(size_t connectionID) = 0;
};

} // namespace meow

#endif
