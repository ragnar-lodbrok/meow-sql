#ifndef DB_PROCEDURE_ENTITY_H
#define DB_PROCEDURE_ENTITY_H

#include <memory>
#include "entity.h"
#include "db/routine_structure.h"

namespace meow {
namespace db {

class DataBaseEntity;

// Intent: procedure or function
class RoutineEntity : public EntityInDatabase
{
public:
    RoutineEntity(const QString & name,
                  Type type = Type::Function,
                  DataBaseEntity * parent = nullptr);

    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return _type; }

    RoutineStructure * structure() const;
    bool hasStructure() const {
        return _structure != nullptr;
    }

    void setName(const QString & name) { _name = name; }
    void setType(const Type type) { _type = type; }

    // Returns a copy (with all internal data)
    RoutineEntity * deepCopy() const;

    virtual void copyDataFrom(const Entity * data) override;

private:
    QString _name;
    Type _type;
    mutable std::unique_ptr<RoutineStructure> _structure;
};


} // namespace db
} // namespace meow

#endif // DB_PROCEDURE_ENTITY_H
