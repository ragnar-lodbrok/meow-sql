#ifndef DB_SESSION_ENTITY_H
#define DB_SESSION_ENTITY_H

#include <memory>
#include <QList>
#include "database_entity.h"

namespace meow {
namespace db {

class ConnectionsManager;
class DataBaseEntity;
class TableEntity;
class User;
class EntityFactory;
class SessionEntity;
using SessionEntityPtr = std::shared_ptr<SessionEntity>;

class SessionEntity : public Entity
{
    Q_OBJECT

private:
    SessionEntity(const std::shared_ptr<Connection> & connection,
                  ConnectionsManager * parent);
public:
    friend class EntityFactory;

    virtual ~SessionEntity() override;

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

    void editEntityInDB(EntityInDatabase * entity, EntityInDatabase * newData);
    bool insertEntityToDB(EntityInDatabase * entity);

    bool dropEntityInDB(EntityInDatabase * entity);
    bool dropDatabase(DataBaseEntity * database);
    void createDatabase(const QString & name,
                        const QString & collation = QString());
    bool editDatabase(DataBaseEntity * database,
                      const QString & newName,
                      const QString & newCollation = QString());

    bool removeEntity(Entity * entity);

    int indexOf(DataBaseEntity * session) const;
    const QList<DataBaseEntityPtr> & databases() const { return _databases; }

    bool isActive() const;
    DataBaseEntity * activeDatabase() const;
    DataBaseEntity * databaseByName(const QString & name) const;

    SessionEntityPtr retain() {
        return std::static_pointer_cast<SessionEntity>(shared_from_this());
    }
    std::shared_ptr<const SessionEntity> retain() const {
        return std::static_pointer_cast<const SessionEntity>(shared_from_this());
    }

    Q_SIGNAL void entityEdited(Entity * entity);
    Q_SIGNAL void entityInserted(Entity * entity);
    Q_SIGNAL void enitityRemoved(Entity * entity);
    Q_SIGNAL void beforeEntityRemoved(Entity * entity);

    Q_SIGNAL void databaseInserted(const DataBaseEntityPtr & database);
    Q_SIGNAL void databaseRemoved(const DataBaseEntityPtr & database);

private:
    ConnectionsManager * connectionsManager() const;
    void initDatabasesListIfNeed();

    void clearAllDatabaseEntities();

    void addEntity(Entity * entity);
    void appendCreatedDatabase(
            const QString & name,
            const QString & afterName = QString());

    std::shared_ptr<Connection> _connection;
    QList<DataBaseEntityPtr> _databases;
    bool _databasesWereInit;
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_ENTITY_H
