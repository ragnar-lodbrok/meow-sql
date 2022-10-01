#include "ssh_tunnel_factory.h"
#ifdef HAS_LIBSSH
#include "libssh_tunnel.h"
#endif
#ifndef Q_OS_WIN
#include "openssh_tunnel.h"
#endif
#include "helpers/logger.h"
#include "db/exception.h"

#include <memory>

namespace meow {
namespace ssh {

std::shared_ptr<ISSHTunnel> SSHTunnelFactory::createTunnel()
{
#ifdef HAS_LIBSSH
    try {
        return std::make_shared<libssh_tunnel>();
    }
    catch (const std::runtime_error& e) {
        failWithError(e.what());
    }
#endif
#ifdef Q_OS_WIN
    return std::make_unique<PLinkSSHTunnel>();
#else
    return std::make_unique<OpenSSHTunnel>();
#endif
}

void SSHTunnelFactory::failWithError(const QString& error)
{
    meowLogC(Log::Category::Error)
        << "SSH Tunnel failed: "
        << error;
    QString errorStr = QObject::tr("SSH Tunnel failed: ") + error;

    throw db::Exception(errorStr);
}


} // namespace ssh
} // namespace meow
