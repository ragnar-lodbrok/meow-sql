#ifndef SSH_OPENSSH_TUNNEL_H
#define SSH_OPENSSH_TUNNEL_H

#include <QProcess>
#include <memory>
#include "db/connection_parameters.h"

namespace meow {
namespace ssh {

class ISSHTunnel
{
public:
    virtual ~ISSHTunnel();

    virtual bool connect(const db::ConnectionParameters & params) = 0;
    virtual void disconnect() = 0;

};

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

    [[ noreturn ]] void failWithError(const QString & error);

    void processOutput(const QString & output);

    QStringList programArguments() const;

    std::unique_ptr<QProcess> _process;

    db::ConnectionParameters _params;

    QString _lastOutputString;

    std::atomic_bool _connected;
    std::atomic_bool _stopWaiting;
};

} // namespace ssh
} // namespace meow

#endif // SSH_OPENSSH_TUNNEL_H
