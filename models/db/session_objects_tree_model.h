#ifndef MODELS_SESSION_OBJECTS_TREE_MODEL_H
#define MODELS_SESSION_OBJECTS_TREE_MODEL_H

#include <QAbstractItemModel>
#include "db/entity/session_entity.h"

namespace meow {
namespace models {
namespace db {

class SessionObjectsTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SessionObjectsTreeModel();
    virtual ~SessionObjectsTreeModel() override;

    void setSession(meow::db::SessionEntity * session);

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
    virtual bool hasChildren(
            const QModelIndex &parent = QModelIndex()) const override;

    struct Object {
        QString databaseName;
        QString entityName;
        QString fieldName;
        meow::db::Entity::Type type = meow::db::Entity::Type::None;
    };

    Object objectAt(const QModelIndex &index);

    Q_SIGNAL void fetchObjectError(const QString & errorMessage);

protected:
    virtual bool canFetchMore(const QModelIndex &parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;

private:

    void reinitItems();
    void removeData();
    void insertData();

    meow::db::SessionEntity * _session;

    class TreeItem;
    TreeItem * _rootItem;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_SESSION_OBJECTS_TREE_MODEL_H
