#ifndef DB_DATABASEENTITY_H
#define DB_DATABASEENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class SessionEntity;
class TableEntity;
class EntityFactory;
class DataBaseEntity;
using DataBaseEntityPtr = std::shared_ptr<DataBaseEntity>;

class DataBaseEntity : public Entity
{
private:
    DataBaseEntity(const QString & dbName, SessionEntity * parent);
public:
    friend class EntityFactory;
    virtual ~DataBaseEntity() override;
    virtual QString name() const override;
    virtual Connection * connection() const override;
    virtual int row() const override;
    virtual int childCount() override;
    virtual Entity * child(int row) override;

    virtual Type type() const override { return Type::Database; }
    virtual QVariant icon() const override;

    virtual db::ulonglong dataSize() const override;
    virtual bool hasDataSize() const override { return true; }

    bool childrenFetched() const;
    void clearChildren();

    int indexOf(Entity * entity);

    bool hasChild(const QString & name, const Entity::Type type);

    // TODO: entity should be shared_ptr?
    void appendEntity(EntityInDatabase * entity);

    bool removeEntity(EntityInDatabase * entity);

    SessionEntity * session() const;

    QString defaultCollation() const {
        return QString(); // TODO
    }

    bool hasEntity(const QString & name,
                   const Entity::Type type = Entity::Type::Table) const;

    DataBaseEntityPtr retain() {
        return std::static_pointer_cast<DataBaseEntity>(shared_from_this());
    }
    std::shared_ptr<const DataBaseEntity> retain() const {
        return std::static_pointer_cast<const DataBaseEntity>(shared_from_this());
    }

private:

    void initEntitiesIfNeed();

    QString _dbName;
    QList<EntityPtr> _entities;
    bool _entitiesWereInit;
};

} // namespace db
} // namespace meow

#endif // DB_DATABASEENTITY_H
