#include "entity_factory.h"
#include "table_entity.h"
#include "view_entity.h"
#include "trigger_entity.h"
#include "routine_entity.h"
#include "database_entity.h"
#include "session_entity.h"

namespace meow {
namespace db {

std::shared_ptr<EntityInDatabase> EntityFactory::createEntityInDatabase(
        const QString & name,
        Entity::Type type,
        DataBaseEntity * parent)
{
    switch (type) {

    case Entity::Type::Table:
        return createTable(name, parent);

    case Entity::Type::View:
        return createView(name, parent);

    case Entity::Type::Trigger:
        return createTrigger(name, parent);

    case Entity::Type::Procedure:
        return createProcedure(name, parent);

    case Entity::Type::Function:
        return createFunction(name, parent);

    default:
        Q_ASSERT(false);
        return nullptr;

    }
}

std::shared_ptr<TableEntity> EntityFactory::createTable(
        const QString & tableName,
        DataBaseEntity * parent)
{
    auto table = std::shared_ptr<TableEntity>(
                new TableEntity(tableName, parent));
    return table;
}

std::shared_ptr<ViewEntity> EntityFactory::createView(
        const QString & viewName,
        DataBaseEntity * parent)
{
    return std::shared_ptr<ViewEntity>(new ViewEntity(viewName, parent));
}

std::shared_ptr<TriggerEntity> EntityFactory::createTrigger(
        const QString & name,
        DataBaseEntity * parent)
{
    return std::shared_ptr<TriggerEntity>(new TriggerEntity(name, parent));
}

std::shared_ptr<RoutineEntity> EntityFactory::createProcedure(
        const QString & name,
        DataBaseEntity * parent)
{
    return std::shared_ptr<RoutineEntity>(new RoutineEntity(
            name,
            Entity::Type::Procedure,
            parent));
}

std::shared_ptr<RoutineEntity> EntityFactory::createFunction(
        const QString & name,
        DataBaseEntity * parent)
{
    return std::shared_ptr<RoutineEntity>(new RoutineEntity(
            name,
            Entity::Type::Function,
            parent));
}

std::shared_ptr<DataBaseEntity> EntityFactory::createDataBase(
        const QString & name,
        SessionEntity * parent)
{
    return std::shared_ptr<DataBaseEntity>(new DataBaseEntity(
            name,
            parent));
}

std::shared_ptr<SessionEntity> EntityFactory::createSession(
        const std::shared_ptr<Connection> & connection,
        ConnectionsManager * parent)
{
    return std::shared_ptr<SessionEntity>(new SessionEntity(
            connection,
            parent));
}


} // namespace db
} // namespace meow
