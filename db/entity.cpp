#include "entity.h"

namespace meow {
namespace db {

Entity::Entity(Connection * connection, Entity * parent /* = nullptr */):
    _connection(connection),
    _parent(parent)
{

}

} // namespace db
} // namespace meow

