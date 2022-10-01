#ifndef SSH_TUNNEL_FACTORY_H
#define SSH_TUNNEL_FACTORY_H

#include "ssh_tunnel_interface.h"
#include <memory>

namespace meow {
namespace ssh {

class SSHTunnelFactory
{
public:
    std::shared_ptr<ISSHTunnel> createTunnel();

private:
    void failWithError(const QString& error);
};

} // namespace ssh
} // namespace meow

#endif // SSH_TUNNEL_FACTORY_H
