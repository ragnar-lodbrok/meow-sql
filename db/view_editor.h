#ifndef DB_VIEW_EDITOR_H
#define DB_VIEW_EDITOR_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class ViewEntity;
class ViewStructure;
class EntityInDatabase;


class ViewEditor
{
public:
    ViewEditor(Connection * connection);
    virtual ~ViewEditor() {}
    virtual bool edit(ViewEntity * view, ViewEntity * newData);
    virtual bool insert(ViewEntity * view);
    virtual bool drop(ViewEntity * view);
protected:
    Connection * _connection;
private:
    QString getSQL(const ViewStructure * view,
                   const QString & viewName,
                   const QString & operation = "CREATE");

};

} // namespace db
} // namespace meow

#endif // DB_VIEW_EDITOR_H
