#include "socket.h"

#include "connection.h"
#include "ISocketReceiver.h"

#include <stdexcept>

using asio::ip::tcp;

namespace meow {
socket::socket(const std::shared_ptr<ISocketReceiver>& receiver)
    : _socket(_ioContext), _receiver(receiver)
{

}

void socket::listen(const std::string& ipAddress, const uint16_t port)
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

void socket::startAccept()
{
    if (!_acceptor.has_value()) {
        throw std::runtime_error("Not listening");
    }
    auto conn = std::make_shared<connection>(_ioContext);
    (*_acceptor).async_accept(conn->socket(), std::bind(&socket::handleAccept, this, conn, std::placeholders::_1));
}

void socket::handleAccept(const std::shared_ptr<connection>& connection, const asio::error_code& error)
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

uint16_t socket::getPort() const
{
    return _port;
}

void socket::pollForMs(uint32_t ms)
{
    _ioContext.run_for(std::chrono::milliseconds(ms));
}
}