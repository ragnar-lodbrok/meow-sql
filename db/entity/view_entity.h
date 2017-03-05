#ifndef DB_VIEW_ENTITY_H
#define DB_VIEW_ENTITY_H

#include "entity.h"

namespace meow {
namespace db {

class DataBaseEntity;

class ViewEntity : public Entity
{
public:
    ViewEntity(const QString & viewName, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::View; }
private:
    QString _viewName;
};


} // namespace db
} // namespace meow

#endif // DB_VIEW_ENTITY_H
