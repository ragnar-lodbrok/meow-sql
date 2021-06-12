#ifndef DB_ENTITY_H
#define DB_ENTITY_H

//#include "db/connection.h"
#include <QString>
#include <QVariant>
#include <QDateTime>
#include <memory>

#include "db/common.h"

// see http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html,
// TreeItem class

// root (ConnectionsManager : Entity)
//   + session (SessionEntity : Entity)
//     + database (DataBaseEntity : Entity)
//       + table/view
//       + func/proc

namespace meow {
namespace db {

class SessionEntity;
class Connection;
class Entity;
using EntityPtr = std::shared_ptr<Entity>;


// Intent: represents any db entity like session, database, table H: TDBObject
class Entity : public QObject, public std::enable_shared_from_this<Entity>
{

protected:
    explicit Entity(Entity * parent = nullptr);
public:

    enum class Type { // keep order
        None,
        Session,
        Database,
        Table,
        View,
        Function,
        Procedure,
        Trigger,
        Event,
        Field, // e.g. Table Column
        COUNT
    };

    virtual ~Entity() override;

    Entity * parent() const { return _parent; }
    void setParent(Entity * parent) { _parent = parent; }

    virtual Connection * connection() const;
    virtual QString name() const { return ""; }
    virtual int childCount() { return 0; }
    virtual Entity * child(int row) { Q_UNUSED(row); return nullptr; }
    virtual int row() const { return 0; }
    virtual Type type() const { return Type::None; }
    virtual QVariant icon() const { return QVariant(); }
    virtual db::ulonglong dataSize() const { return 0; }
    virtual bool hasDataSize() const { return false; }
    virtual void copyDataFrom(const Entity * data);

    bool wasSelected() const { return _wasSelected; }
    void setWasSelected(bool wasSelected) { _wasSelected = wasSelected; }

    bool isNew() const { return _isNew; }
    void setIsNew(bool isNew) { _isNew = isNew; }

    QDateTime created() const { return _created; }
    void setCreated(const QDateTime & created) { _created = created; }

    QDateTime updated() const { return _updated; }
    void setUpdated(const QDateTime & updated) { _updated = updated; }

    QString createCode(bool refresh = false);

    EntityPtr retain() {
        // should be safe if ctor is not public and EntityFabric always returns
        // shared_ptr
        return shared_from_this();
    }
    std::shared_ptr<const Entity> retain() const {
        return shared_from_this();
    }

protected:
    Entity * _parent; // TODO: as weak_ptr ?
private:
    bool _isNew; // is new object to be saved
    bool _wasSelected;
    QDateTime _created;
    QDateTime _updated;
    std::pair<bool, QString> _createCodeCached; // < cached?, data >
};

// -----------------------------------------------------------------------------

class DataBaseEntity;
// Intent: Entity inside Database level
class EntityInDatabase : public Entity
{
protected:
    explicit EntityInDatabase(DataBaseEntity * parent = nullptr);
public:
    DataBaseEntity * dataBaseEntity() const;
    virtual void copyDataFrom(const Entity * data) override;
};

// -----------------------------------------------------------------------------
// Helpers


// finds nearest parent (or itself) entity of passed type (nullptr if not)
// checks until root, ignores children
Entity * findParentEntityOfType(const Entity * entity, Entity::Type type);

SessionEntity * sessionForEntity(const Entity * entity);

// returns count of children of passed type (1 level)
int childCountOfType(Entity * entity, Entity::Type type);


QString databaseName(const Entity * entity);

QString quotedName(const Entity * entity);

QString quotedDatabaseName(const Entity * entity);

// ex. `databaseName`.`entityName`
QString quotedFullName(const Entity * entity);

} // namespace db
} // namespace meow

#endif // DB_ENTITY_H
