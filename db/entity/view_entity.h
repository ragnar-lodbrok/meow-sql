#ifndef DB_VIEW_ENTITY_H
#define DB_VIEW_ENTITY_H

#include <memory>
#include "entity.h"
#include "db/view_structure.h"

namespace meow {
namespace db {

class DataBaseEntity;

class ViewEntity : public EntityInDatabase
{
public:
    ViewEntity(const QString & viewName, DataBaseEntity * parent = nullptr);
    virtual QString name() const override;
    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::View; }

    ViewStructure * structure() const;
    bool hasStructure() const {
        return _structure != nullptr;
    }

private:
    QString _viewName;
    mutable std::unique_ptr<ViewStructure> _structure;
};


} // namespace db
} // namespace meow

#endif // DB_VIEW_ENTITY_H
