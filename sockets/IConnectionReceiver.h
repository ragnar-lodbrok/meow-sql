#pragma once

#pragma once

#include <memory>
#include <system_error>
#include <vector>

namespace meow {
class IConnectionReceiver
{
public:
    virtual void onError(std::error_code code) = 0;

    virtual void onClose() = 0;

    virtual void onData(const std::vector<char>& data, size_t dataLength) = 0;
};
}