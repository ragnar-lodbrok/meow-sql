#ifndef DB_SESSION_ENTITY_H
#define DB_SESSION_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class ConnectionsManager;

class SessionEntity : public Entity
{
public:
    explicit SessionEntity(ConnectionPtr connection, ConnectionsManager * parent);
    virtual QString name() const override;
    virtual Connection * connection() const override;
    virtual int row() const override;
    virtual int childCount() const override;
    virtual Entity * child(int row) const override;
private:
    ConnectionsManager * connectionsManager() const;
    ConnectionPtr _connection;
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_ENTITY_H
