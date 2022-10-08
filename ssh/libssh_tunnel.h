#ifndef SSH_LIBSSH_TUNNEL_H
#define SSH_LIBSSH_TUNNEL_H

#include "sockets/socket.h"
#include "sockets/socket_receiver_interface.h"

#include "libssh.h"
#include "libssh_channel.h"
#include "db/connection_parameters.h"
#include "ssh_tunnel_interface.h"

#include <condition_variable>
#include <thread>
#include <mutex>
#include <unordered_map>

namespace meow {

namespace sockets {
    class Connection;
}

namespace ssh {

class LibSSHConnection;

class LibSSHTunnel : public ISSHTunnel
                   , public sockets::ISocketReceiver
                   , public std::enable_shared_from_this<LibSSHTunnel>
{
public:
    LibSSHTunnel();

    virtual ~LibSSHTunnel() override;

    // <ISSHTunnel>
    virtual bool connect(const db::ConnectionParameters& params) override;

    virtual void disconnect() override;

    virtual bool supportsPassword() const override;

    virtual SSHTunnelParameters params() const override;
    // </ISSHTunnel>
    // <ISocketReceiver>
    virtual std::shared_ptr<sockets::IConnectionReceiver> onNewConnection(
            const std::shared_ptr<sockets::Connection>& connection) override;
    virtual void onConnectionClosed(size_t connectionID) override;
    // </ISocketReceiver>

private:
    void threadFunc();

    static void failWithError(const QString& error);

    QString errorString() const;

    bool verifyHost();

    bool authWithPassword();

    bool authWithPublicKey();

    db::ConnectionParameters _params;
    bool _threadStarted = false;

    std::shared_ptr<LibSSH> _session;
    std::unique_ptr<sockets::Socket> _socket;
    std::thread _thread;
    std::atomic<bool> _stopThread = false;
    bool _threadRunning = false;
    std::condition_variable _threadWait;
    std::mutex _threadMutex;
    using ConnectionPair = std::pair<std::shared_ptr<sockets::Connection>,
                                     std::shared_ptr<LibSSHConnection>>;
    std::unordered_map<size_t, ConnectionPair> _connections;
};

} // namespace ssh
} // namespace meow

#endif
