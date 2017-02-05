#ifndef DB_ENTITY_H
#define DB_ENTITY_H

#include "connection.h"
#include <QString>
#include <QVariant>

// see http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html,
// TreeItem class

// root (ConnectionsManager : Entity)
//   + session (SessionEntity : Entity)
//     + database (DataBaseEntity : Entity)

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
        COUNT
    };

    explicit Entity(Entity * parent = nullptr);
    virtual ~Entity() {}

    Entity * parent() const { return _parent; }

    virtual Connection * connection() const { return nullptr; }
    virtual QString name() const { return ""; }
    virtual int childCount() { return 0; }
    virtual Entity * child(int row) { Q_UNUSED(row); return nullptr; }
    virtual int row() const { return 0; }
    virtual Type type() const { return Type::None; }
    virtual QVariant icon() const { return QVariant(); }

protected:
    Entity * _parent;
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_H
