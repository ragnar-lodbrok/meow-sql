#ifndef SSH_SOCKETS_CONNECTION_H
#define SSH_SOCKETS_CONNECTION_H

#include <asio.hpp>

#include <deque>
#include <memory>
#include <vector>

using asio::ip::tcp;

namespace meow {
namespace ssh {
namespace sockets {

class IConnectionReceiver;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection(asio::io_context& io_context);

    tcp::socket& socket();

    void accept(const std::shared_ptr<IConnectionReceiver>& receiver);

    void write(const std::vector<char>& data);

    void write(const std::vector<char>& data, size_t readLen);

    void readCallback(const asio::error_code& error,
                      size_t bytes_transferred);

    void writeCallback(const asio::error_code& error);

    void closeCallback();

    void close();

    size_t connectionID() const;

private:
    const static size_t MAX_LENGTH = 1024;
    static size_t connectionIDs;

    bool _closed = false;
    tcp::socket _socket;
    std::recursive_mutex _mutex;
    asio::io_context& _ioContext;
    std::weak_ptr<IConnectionReceiver> _receiver;
    std::vector<char> _data;
    std::deque<std::pair<std::vector<char>, size_t>> _writeQueue;
    size_t _connectionID;
};

} // namespace sockets
} // namespace ssh
} // namespace meow

#endif
