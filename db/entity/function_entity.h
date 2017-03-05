#ifndef DB_FUNCTION_ENTITY_H
#define DB_FUNCTION_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class FunctionEntity : public Entity
{
public:
    FunctionEntity(const QString & name, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::Function; }
private:
    QString _name;
};


} // namespace db
} // namespace meow

#endif // DB_FUNCTION_ENTITY_H
