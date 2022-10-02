#include "socket.h"

#include "connection.h"
#include "socket_receiver_interface.h"

#include <stdexcept>

using asio::ip::tcp;

namespace meow {
Socket::Socket(const std::shared_ptr<ISocketReceiver>& receiver)
    : _socket(_ioContext), _receiver(receiver)
{

}

void Socket::listen(const std::string& ipAddress, const uint16_t port)
{
    asio::error_code ec;
    auto ip = asio::ip::address::from_string(ipAddress.c_str(), ec);
    if (ec.value() != 0) {
        throw std::runtime_error("IP address is invalid");
    }
    tcp::endpoint endPoint(ip, port);

    _acceptor = tcp::acceptor(_ioContext, endPoint);
    startAccept();
    auto le = _acceptor->local_endpoint();
    _port = le.port();
}

void Socket::startAccept()
{
    if (!_acceptor.has_value()) {
        throw std::runtime_error("Not listening");
    }
    auto conn = std::make_shared<Connection>(_ioContext);
    (*_acceptor).async_accept(conn->socket(),
                              std::bind(&Socket::handleAccept,
                                        this, conn, std::placeholders::_1));
}

void Socket::handleAccept(
        const std::shared_ptr<Connection>& connection,
        const asio::error_code& error)
{
    if (!error) {
        if (auto receiver = _receiver.lock()) {
            auto connReceiver = receiver->onNewConnection(connection);
            if (connReceiver) {
                connection->accept(connReceiver);
            }
        }
    }
    startAccept();
}

void Socket::pollForMs(uint32_t ms)
{
    _ioContext.run_for(std::chrono::milliseconds(ms));
}
}
