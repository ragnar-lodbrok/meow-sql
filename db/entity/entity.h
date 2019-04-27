#ifndef DB_ENTITY_H
#define DB_ENTITY_H

//#include "db/connection.h"
#include <QString>
#include <QVariant>
#include <QDateTime>

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

// Intent: represents any db entity like session, database, table for tree data
// model usage. H: TDBObject
class Entity : public QObject
{
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
        COUNT
    };

    explicit Entity(Entity * parent = nullptr);
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

    bool wasSelected() const { return _wasSelected; }
    void setWasSelected(bool wasSelected) { _wasSelected = wasSelected; }

    bool isNew() const { return _isNew; }
    void setIsNew(bool isNew) { _isNew = isNew; }

    QDateTime created() const { return _created; }
    void setCreated(const QDateTime & created) { _created = created; }

    QDateTime updated() const { return _updated; }
    void setUpdated(const QDateTime & updated) { _updated = updated; }

    QString createCode(bool refresh = false);

    void copyDataFrom(const Entity * data);

protected:
    Entity * _parent;
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
public:
    explicit EntityInDatabase(DataBaseEntity * parent = nullptr);
    DataBaseEntity * dataBaseEntity() const;
    void copyDataFrom(const EntityInDatabase * data);
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
