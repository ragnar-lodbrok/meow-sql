#ifndef DB_SESSION_ENTITY_H
#define DB_SESSION_ENTITY_H

#include <memory>
#include <QList>
#include "entity.h"

namespace meow {
namespace db {

class ConnectionsManager;
class DataBaseEntity;

class SessionEntity : public Entity
{
public:
    explicit SessionEntity(ConnectionPtr connection, ConnectionsManager * parent);
    virtual ~SessionEntity();

    virtual QString name() const override;
    virtual Connection * connection() const override;
    virtual int row() const override;
    virtual int childCount() override;
    virtual Entity * child(int row) override;
    virtual Type type() const override { return Type::Session; }
    virtual QVariant icon() const override;

    int indexOf(DataBaseEntity * session) const;

private:
    ConnectionsManager * connectionsManager() const;
    void initDatabasesListIfNeed();

    ConnectionPtr _connection;
    QList<DataBaseEntity *> _databases;
    bool _databasesWereInit;
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_ENTITY_H
