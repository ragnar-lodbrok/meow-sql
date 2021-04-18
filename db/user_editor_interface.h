#ifndef DB_USER_EDITOR_INTERFACE_H
#define DB_USER_EDITOR_INTERFACE_H

namespace meow {
namespace db {

class User;
class Connection;

struct UserPasswordRequirements {
    int minLength = 0;
    int minMixedCaseCount = 0;
    int minNumberCount = 0;
    int minSpecialCharCount = 0;
};

class IUserEditor
{
public:
    IUserEditor(Connection * connection) : _connection(connection) {}
    virtual ~IUserEditor() {}
    virtual bool edit(User * user, User * newData) = 0;
    virtual bool insert(User * user) = 0;
    virtual bool drop(User * user) = 0;

    virtual UserPasswordRequirements passwordRequirements() const {
        return {};
    }
protected:
    Connection * _connection;
};

} // namespace db
} // namespace meow

#endif // DB_USER_EDITOR_INTERFACE_H
