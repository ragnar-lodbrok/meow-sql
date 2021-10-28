#ifndef DB_USER_QUERIES_MANAGER_H
#define DB_USER_QUERIES_MANAGER_H

#include <vector>

namespace meow {
namespace db {

class UserQuery;
class ConnectionsManager;

class UserQueriesManager
{
public:
    UserQueriesManager(ConnectionsManager * connectionsManager);
    ~UserQueriesManager();

    void load();
    void save();

    UserQuery * userQueryAt(size_t index);
    int userQueriesCount() const { return _userQueries.size(); }
    UserQuery * appendNewUserQuery();
    bool removeUserQueryAt(size_t index);    
    UserQuery * createUserQueryObject() const;


private:
    ConnectionsManager * _connectionsManager;
    std::vector <UserQuery *> _userQueries;
};

} // namespace db
} // namespace meow

#endif // DB_USER_QUERIES_MANAGER_H
