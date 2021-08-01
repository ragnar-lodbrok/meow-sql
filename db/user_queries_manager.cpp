#include "user_queries_manager.h"
#include "user_query/user_query.h"
#include "settings/queries_storage.h"
#include "app/app.h"

namespace meow {
namespace db {

UserQueriesManager::UserQueriesManager(ConnectionsManager * connectionsManager)
    : _connectionsManager(connectionsManager)
{

}

UserQueriesManager::~UserQueriesManager()
{
    qDeleteAll(_userQueries);
}

void UserQueriesManager::load()
{
    qDeleteAll(_userQueries);

    settings::QueriesStorage * storage
            = meow::app()->settings()->queriesStorage();
    _userQueries = storage->load();

    if (_userQueries.empty()) {
        appendNewUserQuery();
    }
}

void UserQueriesManager::save()
{
    settings::QueriesStorage * storage
            = meow::app()->settings()->queriesStorage();
    storage->save(_userQueries);
}

UserQuery * UserQueriesManager::userQueryAt(size_t index)
{
    Q_ASSERT(index < _userQueries.size());
    return _userQueries.at(index);
}

UserQuery * UserQueriesManager::appendNewUserQuery()
{
    UserQuery * userQuery = createUserQueryObject();
    _userQueries.push_back(userQuery);
    return userQuery;
}

bool UserQueriesManager::removeUserQueryAt(size_t index)
{
    if (index >= _userQueries.size()) {
        return false;
    }
    UserQuery * userQuery = _userQueries.at(index);
    delete userQuery;
    _userQueries.erase(_userQueries.begin() + index); // who did this with C++?
    return true;
}

UserQuery * UserQueriesManager::createUserQueryObject() const
{
    return new UserQuery(_connectionsManager);
}

} // namespace db
} // namespace meow
