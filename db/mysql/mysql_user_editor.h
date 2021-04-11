#ifndef DB_MYSQL_USER_EDITOR_H
#define DB_MYSQL_USER_EDITOR_H

#include "db/user_editor_interface.h"

namespace meow {
namespace db {

class MySQLUserEditor : public IUserEditor
{
public:
    MySQLUserEditor(Connection * connection);
    virtual ~MySQLUserEditor() override {}
    virtual bool edit(User * user, User * newData) override;
    virtual bool insert(User * user) override;
    virtual bool drop(User * user) override;
private:
    bool editLimits(User * user, User * newData);
    bool editPassword(User * user, User * newData);
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_USER_EDITOR_H
