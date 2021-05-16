#ifndef DB_MYSQL_USER_EDITOR_H
#define DB_MYSQL_USER_EDITOR_H

#include <QString>
#include "db/user_editor_interface.h"
#include "db/user_manager.h"

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

    virtual UserPasswordRequirements passwordRequirements() const override;

private:
    bool editLimits(User * user, User * newData);
    bool editPassword(User * user, User * newData);
    bool editPrivileges(User * user, User * newData);

    void editPrivileges(const UserPrivilegePtr & oldPriv,
                        const UserPrivilegePtr & newPriv,
                        User * user);

    void grantPrivileges(const UserPrivilegePtr & priv,
                         const QSet<QString> & privList,
                         User * user);

    void revoke(const QStringList & privList,
                const QString & onObject,
                const QString & fromUser);

    void grant(const QStringList & privList,
               const QString & onObject,
               const QString & toUser);

    bool grantUsageWith(const QStringList & withClauses, User * user);

    void createUser(User * user);

    bool grantLimits(User * user);

    QString privilegeLevelSQL(const UserPrivilegePtr & priv) const;
    QString userHostSQL(const User * user) const;
    QString limitSQLName(const User::LimitType limitType) const;

    int sessionVariableAsInt(const QString & variableName) const;
};

} // namespace db
} // namespace meow

#endif // DB_MYSQL_USER_EDITOR_H
