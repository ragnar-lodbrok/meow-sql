#ifndef MODELS_DB_USER_PRIVILEGES_MODEL_H
#define MODELS_DB_USER_PRIVILEGES_MODEL_H

#include <QAbstractItemModel>
#include "db/user_manager.h"

namespace meow {
namespace models {

namespace forms {
class UserManagementForm;
}

namespace db {

class UserPrivilegesModel : public QAbstractItemModel
{
public:
    explicit UserPrivilegesModel(models::forms::UserManagementForm * form);
    virtual ~UserPrivilegesModel() override;

    void setUser(const meow::db::UserPtr & user);

    virtual QVariant data(const QModelIndex & index,
                          int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QModelIndex index(
            int row, int column,
            const QModelIndex & parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex & index) const override;
    virtual int rowCount(
            const QModelIndex & parent = QModelIndex()) const override;
    virtual int columnCount(
            const QModelIndex & parent = QModelIndex()) const override;
private:

    void reinitItems();
    void removeData();
    void insertData();

    QString labelForPrivilege(
            const meow::db::UserPrivilegePtr & privilege) const;

    QVariant iconForPrivilege(
            const meow::db::UserPrivilegePtr & privilege) const;

    models::forms::UserManagementForm * _form;
    meow::db::UserPtr _user;

    class TreeItem;

    TreeItem * _rootItem;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_USER_PRIVILEGES_MODEL_H
