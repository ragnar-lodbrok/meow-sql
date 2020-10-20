#include "view_structure.h"

namespace meow {
namespace db {

ViewStructure::ViewStructure(ViewEntity * view)
    :_view(view)
{

}

ViewStructure * ViewStructure::deepCopy(ViewEntity * parentView) const
{
    ViewStructure * structure = new ViewStructure(parentView);

    structure->_selectStatement = this->_selectStatement;
    structure->algorithm = this->algorithm;
    structure->definer = this->definer;
    structure->sqlSecurity = this->sqlSecurity;
    structure->checkOption = this->checkOption;

    return structure;
}

bool ViewStructure::operator==(const ViewStructure & other)
{
    return _selectStatement == other._selectStatement
         && algorithm == other.algorithm
         && definer == other.definer
         && sqlSecurity == other.sqlSecurity
         && checkOption == other.checkOption;
}

} // namespace db
} // namespace meow
