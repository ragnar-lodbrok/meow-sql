#include "routine_structure.h"

namespace meow {
namespace db {

RoutineStructure::RoutineStructure(RoutineEntity * routine)
    : deterministic(false)
    , _routine(routine)
{

}

RoutineStructure * RoutineStructure::deepCopy(RoutineEntity * parentRoutine) const
{
    RoutineStructure * structure = new RoutineStructure(parentRoutine);

    structure->body = this->body;
    structure->definer = this->definer;
    structure->returnType = this->returnType;
    structure->dataAccess = this->dataAccess;
    structure->sqlSecurity = this->sqlSecurity;
    structure->comment = this->comment;
    structure->deterministic = this->deterministic;
    structure->params = RoutuneStructureParamList();

    for (const auto & param : this->params) {
        structure->params.append(param);
    }

    return structure;
}

bool RoutineStructure::operator==(const RoutineStructure & other)
{
    return body == other.body
         && definer == other.definer
         && returnType == other.returnType
         && dataAccess == other.dataAccess
         && sqlSecurity == other.sqlSecurity
         && comment == other.comment
         && deterministic == other.deterministic
         && params == other.params;
}

} // namespace db
} // namespace meow
