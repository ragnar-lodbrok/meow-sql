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

bool RoutineStructure::canRemoveParam(int index) const
{
    return index >= 0 && index < params.size();
}

bool RoutineStructure::canRemoveAllParams() const
{
    return params.size() > 0;
}

bool RoutineStructure::canMoveParamUp(int index) const
{
    return index >= 1 && index < params.size();
}

bool RoutineStructure::canMoveParamDown(int index) const
{
    return index >= 0 && index < (params.size() - 1);
}

int RoutineStructure::insertEmptyDefaultParam(int afterIndex)
{
    RoutuneStructureParam newParam;
    newParam.name = QString("Param_%1").arg(params.count() + 1);
    newParam.dataType = "INT";
    newParam.context = "IN";

    if (afterIndex == -1) {
        params.append(newParam);
        return params.count() - 1;
    } else {
        params.insert(afterIndex + 1, newParam);
        return afterIndex + 1;
    }
}

bool RoutineStructure::removeParamAt(int index)
{
    if (canRemoveParam(index)) {
        params.removeAt(index);
        return true;
    }
    return false;
}

void RoutineStructure::removeAllParams()
{
    // Listening: Sojourner - The Deluge
    params.clear();
}

bool RoutineStructure::moveParamUp(int index)
{
    if (canMoveParamUp(index)) {
        params.move(index, index - 1);
        return true;
    }
    return false;
}

bool RoutineStructure::moveParamDown(int index)
{
    if (canMoveParamDown(index)) {
        params.move(index, index + 1);
        return true;
    }
    return false;
}

} // namespace db
} // namespace meow
