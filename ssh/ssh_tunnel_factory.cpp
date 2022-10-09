#include "ssh_tunnel_factory.h"
#ifdef WITH_LIBSSH
#include "libssh_tunnel.h"
#endif
#ifndef Q_OS_WIN
#include "openssh_tunnel.h"
#endif
#include "plink_ssh_tunnel.h"
#include "helpers/logger.h"
#include "db/exception.h"

#include <memory>

namespace meow {
namespace ssh {

std::shared_ptr<ISSHTunnel> SSHTunnelFactory::createTunnel()
{
#ifdef WITH_LIBSSH
    try {
        return std::make_shared<LibSSHTunnel>();
    }
    catch (const std::runtime_error& e) {
        failWithError(e.what());
    }
#else

	#ifdef Q_OS_WIN
		return std::make_shared<PLinkSSHTunnel>();
	#else
		return std::make_shared<OpenSSHTunnel>();
	#endif

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
