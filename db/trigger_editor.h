#ifndef DB_TRIGGER_EDITOR_H
#define DB_TRIGGER_EDITOR_H

#include <QString>

namespace meow {
namespace db {

class Connection;
class TriggerEntity;
class TriggerStructure;

class TriggerEditor
{
public:
    TriggerEditor(Connection * connection);
    virtual ~TriggerEditor() {}
    virtual bool edit(TriggerEntity * trigger, TriggerEntity * newData);
    virtual bool insert(TriggerEntity * trigger);
    virtual bool drop(TriggerEntity * trigger);
protected:
    Connection * _connection;
private:
    QString getCreateSQL(TriggerEntity * trigger);

};

} // namespace db
} // namespace meow

#endif // DB_TRIGGER_EDITOR_H
