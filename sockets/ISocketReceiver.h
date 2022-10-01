#pragma once

#include <memory>

namespace meow {
class connection;

class IConnectionReceiver;

class ISocketReceiver
{
public:
    virtual std::shared_ptr<IConnectionReceiver> onNewConnection(const std::shared_ptr<connection>& connection) = 0;
    virtual void onConnectionClosed(size_t connectionID) = 0;
};
}