#include "entity_list_for_database.h"
#include "entity.h"

namespace meow {
namespace db {

EntityListForDataBase::EntityListForDataBase()
    :_entities()
{

}

EntityListForDataBase::~EntityListForDataBase()
{
    qDeleteAll(_entities);
}

} // namespace db
} // namespace meow
