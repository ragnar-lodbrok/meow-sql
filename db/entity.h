#ifndef DB_ENTITY_H
#define DB_ENTITY_H

#include "connection.h"
#include <QString>

// see http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html,
// TreeItem class

namespace meow {
namespace db {

// Intent: represents any db entity like session, database, table for tree data
// model usage. H: TDBObject
class Entity
{
public:
    explicit Entity(Connection * connection, Entity * parent = nullptr);
    virtual ~Entity() {}

    virtual QString getName() const { return ""; }
    Connection * getConnection() const { return _connection; }

protected:
    Connection * _connection;
    Entity * _parent;
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_H
