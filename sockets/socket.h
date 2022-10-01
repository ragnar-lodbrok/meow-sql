#pragma once

#include <asio.hpp>

#include <optional>

using asio::ip::tcp;

namespace meow {
class ISocketReceiver;

class connection;

class socket
{
public:
    explicit socket(const std::shared_ptr<ISocketReceiver>& receiver);

    void listen(const std::string& hostname, uint16_t port);

    [[nodiscard]] uint16_t getPort() const;

    void pollForMs(uint32_t ms);

private:
    void startAccept();

    void handleAccept(const std::shared_ptr<connection>& connection, const asio::error_code& error);

    asio::io_context _ioContext;
    tcp::socket _socket;
    std::optional<tcp::acceptor> _acceptor;
    std::weak_ptr<ISocketReceiver> _receiver;
    uint16_t _port = 0;
};
}
