#ifndef DB_ROUTINE_STRUCTURE_H
#define DB_ROUTINE_STRUCTURE_H

#include <QString>
#include <QList>

namespace meow {
namespace db {

class RoutineEntity;

class RoutuneStructureParam
{
public:
    QString name;
    QString dataType;
    QString context;

    bool operator==(const RoutuneStructureParam & other)
    {
        return name == other.name
            && dataType == other.dataType
            && context == other.context;
    }
};

using RoutuneStructureParamList = QList<RoutuneStructureParam>;

// Intent: structure for function/procedure
class RoutineStructure
{
public:
    RoutineStructure(RoutineEntity * routine);

    RoutineStructure * deepCopy(RoutineEntity * parentRoutine) const;

    bool operator==(const RoutineStructure & other);

    QString body;
    QString definer;
    QString returnType;
    QString dataAccess;
    QString sqlSecurity;
    QString comment;

    bool deterministic;

    RoutuneStructureParamList params;

    void clear() {
        body = QString();
        definer = QString();
        returnType = QString();
        dataAccess = QString();
        sqlSecurity = QString();
        comment = QString();
        deterministic = false;
        params.clear();
    }

     bool canRemoveParam(int index) const;
     bool canRemoveAllParams() const;
     bool canMoveParamUp(int index) const;
     bool canMoveParamDown(int index) const;

     int insertEmptyDefaultParam(int afterIndex = -1);
     bool removeParamAt(int index);
     void removeAllParams();
     bool moveParamUp(int index);
     bool moveParamDown(int index);

private:
    RoutineEntity * _routine;
};

} // namespace db
} // namespace meow

#endif // DB_ROUTINE_STRUCTURE_H
