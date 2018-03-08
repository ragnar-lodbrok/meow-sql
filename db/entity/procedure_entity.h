#ifndef DB_PROCEDURE_ENTITY_H
#define DB_PROCEDURE_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class ProcedureEntity : public EntityInDatabase
{
public:
    ProcedureEntity(const QString & name, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::Procedure; }
private:
    QString _name;
};


} // namespace db
} // namespace meow

#endif // DB_PROCEDURE_ENTITY_H
