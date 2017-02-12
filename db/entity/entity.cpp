#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Entity * parent /* = nullptr */):
    _parent(parent)
{

}

} // namespace db
} // namespace meow

