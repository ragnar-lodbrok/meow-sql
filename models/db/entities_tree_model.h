#ifndef MODELS_DB_ENTITIESTREEMODEL_H
#define MODELS_DB_ENTITIESTREEMODEL_H

#include <QAbstractItemModel>
#include "db/connections_manager.h"
#include "db/connection.h"

namespace meow {
namespace models {
namespace db {

class EntitiesTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    EntitiesTreeModel(meow::db::ConnectionsManager * dbConnectionsManager,
                      QObject * parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    void onSelectEntityAt(const QModelIndex &index);
    void onEmptySelection();

    QModelIndex indexForEntity(meow::db::Entity * entity);
    meow::db::Entity * currentEntity() const;
    meow::db::SessionEntity * currentSession() const;

    bool canDropCurrentItem() const;

    bool canCreateDatabaseOnCurrentItem() const;
    bool canCreateEntityOnCurrentItem(meow::db::Entity::Type type) const;

    bool allowUserManager() const;

    void createNewEntity(meow::db::Entity::Type type);

    void refreshActiveSession();

    void dropCurrentItem();
    void dropEntity(meow::db::Entity * entity);

    meow::db::ConnectionsManager * dbConnectionsManager() const {
        return _dbConnectionsManager;
    }

private:

    Q_SLOT void onEntityEdited(meow::db::Entity * entity);
    Q_SLOT void onEntityInserted(meow::db::Entity * entity);

    meow::db::Entity * rootItem() const { return _dbConnectionsManager; }

    bool isCurItemDatabaseOrLess() const;

    void onDatabasesDataChanged();

    meow::db::ConnectionsManager * _dbConnectionsManager;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_ENTITIESTREEMODEL_H
