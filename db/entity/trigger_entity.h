#ifndef DB_TRIGGER_ENTITY_H
#define DB_TRIGGER_ENTITY_H

#include <memory>
#include "entity.h"
#include "db/trigger_structure.h"

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

    TriggerStructure * structure() const;
    bool hasStructure() const {
        return _structure != nullptr;
    }

    DataBaseEntity * database() const;

    // Returns a copy (with all internal data)
    TriggerEntity * deepCopy() const;

    virtual void copyDataFrom(const Entity * data) override;


private:
    QString _name;
    mutable std::unique_ptr<TriggerStructure> _structure;
};


} // namespace db
} // namespace meow

#endif // DB_TRIGGER_ENTITY_H
