#ifndef DB_ROUTINE_EDITOR_H
#define DB_ROUTINE_EDITOR_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class RoutineEntity;

class RoutineEditor
{
public:
    RoutineEditor(Connection * connection);
    virtual ~RoutineEditor() {}
    virtual bool edit(RoutineEntity * routine, RoutineEntity * newData);
    virtual bool insert(RoutineEntity * routine);
    virtual bool drop(RoutineEntity * routine);
protected:
    Connection * _connection;
private:
    QString createSQL(RoutineEntity * routine,
                      const QString & routineName);
};

} // namespace db
} // namespace meow

#endif // DB_ROUTINE_EDITOR_H
