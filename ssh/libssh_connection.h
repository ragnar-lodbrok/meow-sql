#pragma once

#include "libssh.h"
#include "libssh_channel.h"
#include "sockets/ISocketReceiver.h"
#include "sockets/IConnectionReceiver.h"
#include "db/connection_parameters.h"
#include <sockets/connection.h>

#include <memory>
#include <condition_variable>
#include <thread>
#include <mutex>

namespace meow::ssh {
class libssh_connection: public IConnectionReceiver
{
public:
    libssh_connection(
        db::ConnectionParameters params,
        std::shared_ptr<libssh> session,
        std::unique_ptr<libssh_channel> channel,
        std::shared_ptr<connection> connection,
        const std::shared_ptr<ISocketReceiver>& receiver
    );

    ~libssh_connection();

    // <IConnectionReceiver>
    void onError(std::error_code code) override;

    void onClose() override;

    void onData(const std::vector<char>& data, size_t dataLength) override;
    // </IConnectionReceiver>

    void startTunnel();

    void close();

private:
    int openForward(const std::unique_ptr<libssh_channel>& channel);
    void threadFunc();

    db::ConnectionParameters _params;
    std::unique_ptr<libssh_channel> _channel;
    std::shared_ptr<libssh> _session;
    std::shared_ptr<connection> _connection;
    std::weak_ptr<ISocketReceiver> _receiver;

    std::thread _thread;
    std::atomic<bool> _stopThread = false;
    bool _threadRunning = false;
    std::condition_variable _threadWait;
    std::mutex _threadMutex;
};
}