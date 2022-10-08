#ifndef SSH_SOCKETS_SOCKET_H
#define SSH_SOCKETS_SOCKET_H

#include <asio.hpp>

using asio::ip::tcp;

namespace meow {
namespace ssh {
namespace sockets {

class ISocketReceiver;

class Connection;

class Socket
{
public:
    explicit Socket(const std::shared_ptr<ISocketReceiver>& receiver);

    void listen(const std::string& hostname, uint16_t port);

    uint16_t port() const { return _port; }

    void pollForMs(uint32_t ms);

private:
    void startAccept();

    void handleAccept(const std::shared_ptr<Connection>& connection,
                      const asio::error_code& error);

    asio::io_context _ioContext;
    tcp::socket _socket;
    std::unique_ptr<tcp::acceptor> _acceptor;
    std::weak_ptr<ISocketReceiver> _receiver;
    uint16_t _port;
};

} // namespace sockets
} // namespace ssh
} // namespace meow

#endif
