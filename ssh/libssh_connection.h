#ifndef SSH_LIBSSH_CONNECTION_H
#define SSH_LIBSSH_CONNECTION_H

#include "libssh.h"
#include "libssh_channel.h"
#include "sockets/socket_receiver_interface.h"
#include "sockets/connection_receiver_interface.h"
#include "db/connection_parameters.h"
#include "sockets/connection.h"

#include <memory>
#include <condition_variable>
#include <thread>
#include <mutex>

namespace meow {
namespace ssh {

class LibSSHConnection : public sockets::IConnectionReceiver
{
public:
    LibSSHConnection(
        db::ConnectionParameters params,
        std::shared_ptr<LibSSH> session,
        std::unique_ptr<LibSSHChannel> channel,
        std::shared_ptr<sockets::Connection> connection,
        const std::shared_ptr<sockets::ISocketReceiver>& receiver
    );

    virtual ~LibSSHConnection() override;

    // <IConnectionReceiver>
    virtual void onError(std::error_code code) override;

    virtual void onClose() override;

    virtual void onData(const std::vector<char>& data,
                        size_t dataLength) override;
    // </IConnectionReceiver>

    void startTunnel();

    void close();

private:
    int openForward(const std::unique_ptr<LibSSHChannel>& channel);
    void threadFunc();

    db::ConnectionParameters _params;
    std::unique_ptr<LibSSHChannel> _channel;
    std::shared_ptr<LibSSH> _session;
    std::shared_ptr<sockets::Connection> _connection;
    std::weak_ptr<sockets::ISocketReceiver> _receiver;

    std::thread _thread;
    std::atomic<bool> _stopThread;
    bool _threadRunning = false;
    std::condition_variable _threadWait;
    std::mutex _threadMutex;
};

} // namespace ssh
} // namespace meow

#endif
