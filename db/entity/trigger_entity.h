#ifndef DB_TRIGGER_ENTITY_H
#define DB_TRIGGER_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class TriggerEntity : public EntityInDatabase
{
public:
    TriggerEntity(const QString & name, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::Trigger; }
private:
    QString _name;
};


} // namespace db
} // namespace meow

#endif // DB_TRIGGER_ENTITY_H
