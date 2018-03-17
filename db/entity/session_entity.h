#ifndef DB_SESSION_ENTITY_H
#define DB_SESSION_ENTITY_H

#include <memory>
#include <QList>
#include "entity.h"

namespace meow {
namespace db {

class ConnectionsManager;
class DataBaseEntity;
class TableEntity;

class SessionEntity : public QObject, public Entity
{
    Q_OBJECT

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

    virtual db::ulonglong dataSize() const override;
    virtual bool hasDataSize() const override { return true; }

    void refreshAllEntities();

    void editTableInDB(TableEntity * table, TableEntity * newData);
    void insertTableToDB(TableEntity * table);
    bool dropEntityInDB(EntityInDatabase * entity);

    int indexOf(DataBaseEntity * session) const;

    Q_SIGNAL void entityEdited(Entity * entity);
    Q_SIGNAL void entityInserted(Entity * entity);

private:
    ConnectionsManager * connectionsManager() const;
    void initDatabasesListIfNeed();

    void clearAllDatabaseEntities();

    void addEntity(Entity * entity);
    bool removeEntity(Entity * entity);

    ConnectionPtr _connection;
    QList<DataBaseEntity *> _databases;
    bool _databasesWereInit;
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_ENTITY_H
