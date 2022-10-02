#include "connection.h"

#include "connection_receiver_interface.h"

namespace meow {

size_t Connection::connectionIDs = 0;

Connection::Connection(asio::io_context& ioContext)
    : _ioContext(ioContext)
    , _socket(ioContext)
    , _data(std::vector<char>(Connection::MAX_LENGTH))
    , _connectionID(Connection::connectionIDs++)
{

}

tcp::socket& Connection::socket()
{
    return _socket;
}

void Connection::accept(const std::shared_ptr<IConnectionReceiver>& receiver)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    _receiver = receiver;
    if (!_closed) {
        _socket.async_read_some(
                    asio::buffer(_data.data(), Connection::MAX_LENGTH),
                    std::bind(&Connection::readCallback, shared_from_this(),
                            std::placeholders::_1,
                            std::placeholders::_2));
    }
}

void Connection::readCallback(
        const asio::error_code& error, size_t bytes_transferred)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    if (error) {
        if (const auto receiver = _receiver.lock()) {
            receiver->onError(error);
        }
        close();
        return;
    }

    if (auto receiver = _receiver.lock()) {
        receiver->onData(_data, bytes_transferred);
    }

    if (!_closed) {
        _socket.async_read_some(
                asio::buffer(_data.data(), Connection::MAX_LENGTH),
                std::bind(&Connection::readCallback, shared_from_this(),
                        std::placeholders::_1,
                        std::placeholders::_2));
    }
}

void Connection::write(const std::vector<char>& data)
{
    write(data, data.size());
}

void Connection::write(const std::vector<char>& data, size_t len)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    bool writeInProgress = !_writeQueue.empty();
    _writeQueue.push_back(std::make_pair(data, len));
    if (!_closed && !writeInProgress) {
        asio::async_write(_socket,
                          asio::buffer(_writeQueue.front().first.data(),
                                       _writeQueue.front().second),
                          std::bind(&Connection::writeCallback,
                                    shared_from_this(),
                                    std::placeholders::_1));
    }
}

void Connection::writeCallback(const asio::error_code& error)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    if (error) {
        if (const auto receiver = _receiver.lock()) {
            receiver->onError(error);
        }
        close();
        return;
    }

    _writeQueue.pop_front();
    if (!_closed && !_writeQueue.empty()) {
        asio::async_write(_socket,
                          asio::buffer(_writeQueue.front().first.data(),
                                       _writeQueue.front().second),
                          std::bind(&Connection::writeCallback,
                                    shared_from_this(),
                                    std::placeholders::_1));
    }
}

void Connection::close()
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    if (_closed) {
        return;
    }
    _closed = true;
    asio::post(_ioContext,
               std::bind(&Connection::closeCallback, shared_from_this()));
}

void Connection::closeCallback()
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    _socket.close();
    if (auto receiver = _receiver.lock()) {
        receiver->onClose();
    }
}

size_t Connection::connectionID() const
{
    return _connectionID;
}

}
