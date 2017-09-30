#include "foreign_key.h"

namespace meow {
namespace db {


ForeignKey::ForeignKey()
    :_onUpdate(ReferenceOption::NoAction),
     _onDelete(ReferenceOption::NoAction)
{

}

} // namespace db
} // namespace meow
