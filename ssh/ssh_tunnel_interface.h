#ifndef SSH_TUNNEL_INTERFACE_H
#define SSH_TUNNEL_INTERFACE_H

namespace meow {

namespace db {
class ConnectionParameters;
}

namespace ssh {

class ISSHTunnel
{
public:
    virtual ~ISSHTunnel() {}

    virtual bool connect(const db::ConnectionParameters & params) = 0;
    virtual void disconnect() = 0;

};

} // namespace ssh
} // namespace meow

#endif // SSH_TUNNEL_INTERFACE_H
