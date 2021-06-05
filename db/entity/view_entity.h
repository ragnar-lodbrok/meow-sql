#ifndef DB_VIEW_ENTITY_H
#define DB_VIEW_ENTITY_H

#include <memory>
#include "entity.h"
#include "db/view_structure.h"

namespace meow {
namespace db {

class DataBaseEntity;
class EntityFactory;
class ViewEntity;
using ViewEntityPtr = std::shared_ptr<ViewEntity>;

class ViewEntity : public EntityInDatabase
{
private:
    ViewEntity(const QString & viewName, DataBaseEntity * parent = nullptr);
public:
    friend class EntityFactory;
    virtual QString name() const override;
    void setName(const QString & name) { _viewName = name; }

    virtual QVariant icon() const override;
    virtual Type type() const override { return Type::View; }

    ViewStructure * structure() const;
    bool hasStructure() const {
        return _structure != nullptr;
    }

    DataBaseEntity * database() const;

    // Returns a copy (with all internal data)
    ViewEntityPtr deepCopy() const;

    virtual void copyDataFrom(const Entity * data) override;

private:
    QString _viewName;
    mutable std::unique_ptr<ViewStructure> _structure;
};


} // namespace db
} // namespace meow

#endif // DB_VIEW_ENTITY_H
