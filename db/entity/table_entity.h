#ifndef DB_TABLE_ENTITY_H
#define DB_TABLE_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class TableEntity : public Entity
{
public:
    TableEntity(const QString & tableName, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::Table; }
private:
    QString _tableName;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENTITY_H
