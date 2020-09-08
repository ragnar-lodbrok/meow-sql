#ifndef SSH_LIBSSH_TUNNEL_H
#define SSH_LIBSSH_TUNNEL_H

#include <libssh/libssh.h>
#include <poll.h>
#include "db/connection_parameters.h"
#include "ssh_tunnel_interface.h"

namespace meow {
namespace ssh {

// Not finished, doesn't work and probably will never work =)
class LibSSHTunnel : public ISSHTunnel
{
public:
    LibSSHTunnel();

    virtual ~LibSSHTunnel() override;

    virtual bool connect(const db::ConnectionParameters & params) override;
    virtual void disconnect() override;

    db::ConnectionParameters * connectionParams() {
        return &_params;
    }

private:
    db::ConnectionParameters _params;

    QString errorString() const;

    bool verifyHost();

    bool authWithPassword();
    
    bool authWithPublicKey();

    int openForward();

    int createSocket();

    static void* threadFunc(void* data);

    void* threadFunc();

    bool acceptNewSocket();
    bool openTunnel();

    void forwardToSSH();
    void forwardFromSSH();
    void forwardFromSSH2();

    ssh_session _session;
    ssh_channel _forwardingChannel;

    pthread_t _thread;

    int _wakeupSocket; // TODO: accept socket
    int _clientSocket;

    pollfd fds[3];
    size_t fdsNumber;

    std::atomic<bool> _stopThread;
};

} // namespace ssh
} // namespace meow

#endif // SSH_LIBSSH_TUNNEL_H
