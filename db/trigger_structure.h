#ifndef DB_TRIGGER_STRUCTURE_H
#define DB_TRIGGER_STRUCTURE_H
#include <QString>

namespace meow {
namespace db {

class TriggerEntity;

// Intent: holds trigger options/fields
class TriggerStructure
{
public:
    TriggerStructure(TriggerEntity * trigger);

    QString definer() const { return _definer; }
    QString tableName() const { return _tableName; }
    QString actionTime() const { return _actionTime; } // BEFORE | AFTER
    QString event() const { return _event; } // INSERT | UPDATE | DELETE
    QString statement() const { return _statement; }

    void setDefiner(const QString & value) { _definer = value; }
    void setTableName(const QString & value) { _tableName = value; }
    void setActionTime(const QString & value) { _actionTime = value; }
    void setEvent(const QString & value) { _event = value; }
    void setStatement(const QString & value) { _statement = value; }

    void clear() {
        _definer = QString();
        _tableName = QString();
        _actionTime = QString();
        _event = QString();
        _statement = QString();
    }

    TriggerStructure * deepCopy(TriggerEntity * parentTrigger) const;

    bool operator==(const TriggerStructure & other);

private:
    TriggerEntity * _trigger;
    QString _definer;
    QString _tableName;
    QString _actionTime;
    QString _event;
    QString _statement;

};

} // namespace db
} // namespace meow

#endif // DB_TRIGGER_STRUCTURE_H
