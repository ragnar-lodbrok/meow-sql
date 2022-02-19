#ifndef SSH_TUNNEL_FACTORY_H
#define SSH_TUNNEL_FACTORY_H

#include "ssh_tunnel_interface.h"
#include <memory>

namespace meow {
namespace ssh {

class SSHTunnelFactory
{
public:
    std::unique_ptr<ISSHTunnel> createTunnel();

};

} // namespace ssh
} // namespace meow

#endif // SSH_TUNNEL_FACTORY_H
