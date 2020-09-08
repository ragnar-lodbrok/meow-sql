#ifndef SSH_OPENSSH_TUNNEL_H
#define SSH_OPENSSH_TUNNEL_H

#include <QProcess>
#include "db/connection_parameters.h"
#include "ssh_tunnel_interface.h"

namespace meow {
namespace ssh {

class OpenSSHTunnel : public QObject, public ISSHTunnel
{
public:
    OpenSSHTunnel();

    virtual ~OpenSSHTunnel() override;

    virtual bool connect(const db::ConnectionParameters & params) override;
    virtual void disconnect() override;

private:

    Q_SLOT void onProcessError(QProcess::ProcessError error);
    Q_SLOT void onProcessStdOut();
    Q_SLOT void onProcessStdErr();

    QString errorString() const;

    void failWithError(const QString & error);

    void processOutput(const QString & output);

    QStringList programArguments() const;

    std::unique_ptr<QProcess> _process;

    db::ConnectionParameters _params;

    QString _lastOutputString;

    bool _connected;
    bool _stopWaiting;
};

} // namespace ssh
} // namespace meow

#endif // SSH_OPENSSH_TUNNEL_H
