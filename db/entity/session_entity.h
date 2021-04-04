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
class User;

class SessionEntity : public Entity
{
    Q_OBJECT

public:
    SessionEntity(std::shared_ptr<Connection> connection,
                  ConnectionsManager * parent);
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
    const QList<DataBaseEntity *> & databases() const { return _databases; }

    bool isActive() const;
    DataBaseEntity * activeDatabase() const;
    DataBaseEntity * databaseByName(const QString & name) const;

    Q_SIGNAL void entityEdited(Entity * entity);
    Q_SIGNAL void entityInserted(Entity * entity);
    Q_SIGNAL void enitityRemoved(Entity * entity);
    Q_SIGNAL void beforeEntityRemoved(Entity * entity);

private:
    ConnectionsManager * connectionsManager() const;
    void initDatabasesListIfNeed();

    void clearAllDatabaseEntities();

    void addEntity(Entity * entity);
    void appendCreatedDatabase(
            const QString & name,
            const QString & afterName = QString());

    std::shared_ptr<Connection> _connection;
    QList<DataBaseEntity *> _databases;
    bool _databasesWereInit;
};

} // namespace db
} // namespace meow

#endif // DB_SESSION_ENTITY_H
