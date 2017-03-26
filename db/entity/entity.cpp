#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Entity * parent /* = nullptr */):
    _parent(parent),
    _wasSelected(false)
{

}

Connection * Entity::connection() const
{
    if (_parent && _parent->type() != Type::None) {
        return _parent->connection();
    }
    return nullptr;
}

Entity * Entity::findParentOfType(Type type) const
{
    if (type == Type::None) {
        return nullptr;
    } else if (this->type() == type) {
        return const_cast<Entity *>(this);
    } else if (_parent) {
        return _parent->findParentOfType(type);
    } else {
        return nullptr;
    }
}

} // namespace db
} // namespace meow

