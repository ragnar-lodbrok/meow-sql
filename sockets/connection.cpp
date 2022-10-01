#include "connection.h"

#include "IConnectionReceiver.h"

namespace meow {
size_t connection::connectionIDs = 0;

connection::connection(asio::io_context& ioContext)
    : _ioContext(ioContext)
    , _socket(ioContext)
    , _data(std::vector<char>(connection::MAX_LENGTH))
    , _connectionID(connection::connectionIDs++)
{

}

tcp::socket& connection::socket()
{
    return _socket;
}

void connection::accept(const std::shared_ptr<IConnectionReceiver>& receiver)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    _receiver = receiver;
    if (!_closed) {
        _socket.async_read_some(asio::buffer(_data.data(), connection::MAX_LENGTH),
                                std::bind(&connection::readCallback, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
}

void connection::readCallback(const asio::error_code& error, size_t bytes_transferred)
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
        _socket.async_read_some(asio::buffer(_data.data(), connection::MAX_LENGTH),
                                std::bind(&connection::readCallback, shared_from_this(),
                                          std::placeholders::_1,
                                          std::placeholders::_2));
    }
}

void connection::write(const std::vector<char>& data)
{
    write(data, data.size());
}

void connection::write(const std::vector<char>& data, size_t len)
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    bool writeInProgress = !_writeQueue.empty();
    _writeQueue.push_back(std::make_pair(data, len));
    if (!_closed && !writeInProgress) {
        asio::async_write(_socket,
                          asio::buffer(_writeQueue.front().first.data(),
                                       _writeQueue.front().second),
                          std::bind(&connection::writeCallback, shared_from_this(),
                                    std::placeholders::_1));
    }
}

void connection::writeCallback(const asio::error_code& error)
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
                          std::bind(&connection::writeCallback, shared_from_this(),
                                    std::placeholders::_1));
    }
}

void connection::close()
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    if (_closed) {
        return;
    }
    _closed = true;
    asio::post(_ioContext,
               std::bind(&connection::closeCallback, shared_from_this()));
}

void connection::closeCallback()
{
    std::unique_lock<std::recursive_mutex> lk(_mutex);
    _socket.close();
    if (auto receiver = _receiver.lock()) {
        receiver->onClose();
    }
}

size_t connection::connectionID() const
{
    return _connectionID;
}
}
