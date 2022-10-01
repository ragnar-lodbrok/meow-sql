#pragma once

#include "sockets/socket.h"
#include "sockets/ISocketReceiver.h"

#include "libssh.h"
#include "libssh_channel.h"
#include "db/connection_parameters.h"
#include "ssh_tunnel_interface.h"

#include <condition_variable>
#include <thread>
#include <mutex>
#include <unordered_map>

namespace meow {
class connection;
namespace ssh {
class libssh_connection;
class libssh_tunnel : public ISSHTunnel, public ISocketReceiver, public std::enable_shared_from_this<libssh_tunnel>
{
public:
    libssh_tunnel();

    ~libssh_tunnel() override;

    // <ISSHTunnel>
    bool connect(const db::ConnectionParameters& params) override;

    void disconnect() override;

    bool supportsPassword() const override;

    SSHTunnelParameters params() const override;
    // </ISSHTunnel>
    // <ISocketReceiver>
    std::shared_ptr<IConnectionReceiver> onNewConnection(const std::shared_ptr<connection>& connection) override;
    void onConnectionClosed(size_t connectionID) override;
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

    std::shared_ptr<libssh> _session;
    std::unique_ptr<socket> _socket;
    std::thread _thread;
    std::atomic<bool> _stopThread = false;
    bool _threadRunning = false;
    std::condition_variable _threadWait;
    std::mutex _threadMutex;
    std::unordered_map<size_t, std::pair<std::shared_ptr<connection>, std::shared_ptr<libssh_connection>>> _connections;
};
}
}
