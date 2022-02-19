#include "ssh_tunnel_factory.h"
#include "openssh_tunnel.h"

#ifdef Q_OS_WIN
#include "plink_ssh_tunnel.h"
#endif

namespace meow {
namespace ssh {

std::unique_ptr<ISSHTunnel> SSHTunnelFactory::createTunnel()
{
#ifdef Q_OS_WIN
    std::unique_ptr<ISSHTunnel> tunnel(new PLinkSSHTunnel());
#else
    std::unique_ptr<ISSHTunnel> tunnel(new OpenSSHTunnel());
#endif

    return tunnel;
}


} // namespace ssh
} // namespace meow
