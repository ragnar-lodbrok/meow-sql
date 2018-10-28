#include "editable_grid_data.h"

namespace meow {
namespace db {

EditableGridData::EditableGridData()
{

}

void EditableGridData::reserve(int alloc)
{
    _rows.reserve(alloc);
}

void EditableGridData::reserveForAppend(int append)
{
    _rows.reserve(_rows.size() + append);
}

} // namespace db
} // namespace meow
