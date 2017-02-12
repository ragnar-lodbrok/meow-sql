#ifndef DB_TABLE_ENTITY_H
#define DB_TABLE_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class TableEntity : public Entity
{
public:
    TableEntity(const QString & tableName, DataBaseEntity * parent);
    virtual QString name() const override;

private:
    QString _tableName;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENTITY_H
