#ifndef DB_ENTITY_H
#define DB_ENTITY_H

#include "db/connection.h"
#include <QString>
#include <QVariant>
#include <QDateTime>

// see http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html,
// TreeItem class

// root (ConnectionsManager : Entity)
//   + session (SessionEntity : Entity)
//     + database (DataBaseEntity : Entity)
//       + table/view
//       + func/proc

namespace meow {
namespace db {

// Intent: represents any db entity like session, database, table for tree data
// model usage. H: TDBObject
class Entity // TODO: :public QObject
{
public:

    enum class Type {
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
    virtual ~Entity() {}

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

    QDateTime created() const { return _created; }
    void setCreated(const QDateTime & created) { _created = created; }

    QDateTime updated() const { return _updated; }
    void setUpdated(const QDateTime & updated) { _updated = updated; }

protected:
    Entity * _parent;
private:
    bool _wasSelected;
    QDateTime _created;
    QDateTime _updated;
};


// -----------------------------------------------------------------------------
// Helpers


// finds nearest parent (or itself) entity of passed type (nullptr if not)
// checks until root, ignores children
Entity * findParentEntityOfType(Entity * entity, Entity::Type type);

// returns count of children of passed type (1 level)
int childCountOfType(Entity * entity, Entity::Type type);


QString databaseName(Entity * entity);

QString quotedName(Entity * entity);

QString quotedDatabaseName(Entity * entity);

// ex. `databaseName`.`entityName`
QString quotedFullName(Entity * entity);

} // namespace db
} // namespace meow

#endif // DB_ENTITY_H
