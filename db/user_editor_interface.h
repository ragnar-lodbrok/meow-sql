#ifndef DB_USER_EDITOR_INTERFACE_H
#define DB_USER_EDITOR_INTERFACE_H

namespace meow {
namespace db {

class User;
class Connection;

class IUserEditor
{
public:
    IUserEditor(Connection * connection) : _connection(connection) {}
    virtual ~IUserEditor() {}
    virtual bool edit(User * user, User * newData) = 0;
    virtual bool insert(User * user) = 0;
    virtual bool drop(User * user) = 0;
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_USER_EDITOR_INTERFACE_H
