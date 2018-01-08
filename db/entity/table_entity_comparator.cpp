#include "table_entity_comparator.h"
#include "table_entity.h"

namespace meow {
namespace db {

TableEntityComparator::TableEntityComparator()
    :_prev(nullptr),
     _curr(nullptr)
{

}

bool TableEntityComparator::nameDiffers() const
{
    // Listening: Be´lakor - Roots to Sever

    QString namePrev = _prev ? _prev->name() : QString();
    QString nameCurr = _curr ? _curr->name() : QString();

    return namePrev != nameCurr;
}

} // namespace db
} // namespace meow
