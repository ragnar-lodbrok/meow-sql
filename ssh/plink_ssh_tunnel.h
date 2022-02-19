#ifndef SSH_PLINK_SSH_TUNNEL_H
#define SSH_PLINK_SSH_TUNNEL_H

#include <QProcess>
#include "db/connection_parameters.h"
#include "ssh_tunnel_interface.h"

namespace meow {
namespace ssh {

// SSH tunnel via Putty's plink (must be in PATH)
// Download it here: https://www.chiark.greenend.org.uk/~sgtatham/putty/latest.html
class PLinkSSHTunnel : public QObject, public ISSHTunnel
{
public:
    PLinkSSHTunnel();

    virtual ~PLinkSSHTunnel() override;

    virtual bool connect(const db::ConnectionParameters & params) override;
    virtual void disconnect() override;
    virtual bool supportsPassword() const override { return true; }
    virtual SSHTunnelParameters params() const override;

private:

    class Impl;
    Impl * _impl;
};

} // namespace ssh
} // namespace meow

#endif // SSH_PLINK_SSH_TUNNEL_H
