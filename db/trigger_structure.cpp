#include "trigger_structure.h"

namespace meow {
namespace db {

TriggerStructure::TriggerStructure(TriggerEntity * trigger)
    : _trigger(trigger)
{

}

TriggerStructure * TriggerStructure::deepCopy(TriggerEntity * parentTrigger) const
{
    TriggerStructure * structure = new TriggerStructure(parentTrigger);

    structure->_definer = this->_definer;
    structure->_tableName = this->_tableName;
    structure->_actionTime = this->_actionTime;
    structure->_event = this->_event;
    structure->_statement = this->_statement;

    return structure;
}

bool TriggerStructure::operator==(const TriggerStructure & other)
{
    return _definer == other._definer
         && _tableName == other._tableName
         && _actionTime == other._actionTime
         && _event == other._event
         && _statement == other._statement;
}

} // namespace db
} // namespace meow
