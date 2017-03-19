#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Entity * parent /* = nullptr */):
    _parent(parent),
    _wasSelected(false)
{

}

} // namespace db
} // namespace meow

