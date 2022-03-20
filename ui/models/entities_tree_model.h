#ifndef MODELS_DB_ENTITIESTREEMODEL_H
#define MODELS_DB_ENTITIESTREEMODEL_H

#include <QAbstractItemModel>
#include "db/connections_manager.h"
#include "db/connection.h"

namespace meow {
namespace ui {
namespace models {

class EntitiesTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    enum class Columns {
        Name = 0,
        Count
    };

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
    bool removeRows(int row,
                    int count,
                    const QModelIndex &parent = QModelIndex()) override;

    void onSelectEntityAt(const QModelIndex &index);
    void onEmptySelection();

    QModelIndex indexForEntity(meow::db::Entity * entity);
    meow::db::Entity * currentEntity() const;
    meow::db::SessionEntity * currentSession() const;

    bool canDropCurrentItem() const;
    bool canEmptyCurrentItem() const;

    bool canCreateDatabaseOnCurrentItem() const;
    bool canCreateEntityOnCurrentItem(meow::db::Entity::Type type) const;

    bool allowUserManager() const;

    void createNewEntity(meow::db::Entity::Type type);

    void refreshActiveSession();
    void reloadData();

    void dropCurrentItem();
    void dropEntity(meow::db::Entity * entity);

    void emptyCurrentItem();

    meow::db::ConnectionsManager * dbConnectionsManager() const {
        return _dbConnectionsManager;
    }

    Q_SIGNAL void loadDataError(const QString & errorMessage);

    bool filterAcceptsRow(int row,
                          const QModelIndex & parent,
                          const QRegularExpression & databaseFilter,
                          const QRegularExpression & tableFilter) const;

protected:
    virtual bool canFetchMore(const QModelIndex &parent) const override;
    virtual void fetchMore(const QModelIndex &parent) override;

private:

    Q_SLOT void onConnectionOpened(meow::db::SessionEntity * session);
    Q_SLOT void onConnectionClosed(meow::db::SessionEntity * session);

    Q_SLOT void onEntityEdited(meow::db::Entity * entity);
    Q_SLOT void onEntityInserted(meow::db::Entity * entity);

    Q_SLOT void onDatabasesDataChanged();

    void reinitItems();
    void removeData();
    void insertData();

    class TreeItem;

    TreeItem * rootItem() const { return _rootItem.get(); }

    TreeItem * itemForEntity(meow::db::Entity * entity);

    bool isCurItemDatabaseOrLess() const;

    meow::db::ConnectionsManager * _dbConnectionsManager;

    std::shared_ptr<TreeItem> _rootItem;
};

} // namespace models
} // namespace ui
} // namespace meow

#endif // MODELS_DB_ENTITIESTREEMODEL_H
