#ifndef DB_ENTITY_FACTORY_H
#define DB_ENTITY_FACTORY_H

#include <QString>
#include <memory>
#include "entity.h"

namespace meow {
namespace db {

class TableEntity;
class ViewEntity;
class TriggerEntity;
class RoutineEntity;
class TriggerEntity;
class DataBaseEntity;
class SessionEntity;
class Connection;
class ConnectionsManager;

class EntityFactory
{
public:

    static std::shared_ptr<EntityInDatabase> createEntityInDatabase(
            const QString & name,
            Entity::Type type,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<TableEntity> createTable(
            const QString & tableName,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<ViewEntity> createView(
            const QString & viewName,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<TriggerEntity> createTrigger(
            const QString & name,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<RoutineEntity> createProcedure(
            const QString & name,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<RoutineEntity> createFunction(
            const QString & name,
            DataBaseEntity * parent = nullptr);

    static std::shared_ptr<RoutineEntity> createRoutine(
            const QString & name,
            Entity::Type type,
            DataBaseEntity * parent = nullptr) {

        if (type == Entity::Type::Function) {
            return createFunction(name, parent);
        } else {
            return createProcedure(name, parent);
        }
    }

    static std::shared_ptr<DataBaseEntity> createDataBase(
            const QString & name,
            SessionEntity * parent = nullptr);

    static std::shared_ptr<SessionEntity> createSession(
            const std::shared_ptr<Connection> & connection,
            ConnectionsManager * parent);    
};

} // namespace db
} // namespace meow

#endif // DB_ENTITY_FACTORY_H
