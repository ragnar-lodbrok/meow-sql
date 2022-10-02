#ifndef SSH_SOCKETS_CONNECTION_RECEIVER_INTERFACE_H
#define SSH_SOCKETS_CONNECTION_RECEIVER_INTERFACE_H

#include <memory>
#include <system_error>
#include <vector>

namespace meow {

class IConnectionReceiver
{
public:

    virtual ~IConnectionReceiver() {}

    virtual void onError(std::error_code code) = 0;

    virtual void onClose() = 0;

    virtual void onData(const std::vector<char>& data, size_t dataLength) = 0;
};

} // namespace meow

#endif
