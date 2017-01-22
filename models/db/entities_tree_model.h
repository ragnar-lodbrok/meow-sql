#ifndef MODELS_DB_ENTITIESTREEMODEL_H
#define MODELS_DB_ENTITIESTREEMODEL_H

#include <QAbstractItemModel>
#include "db/connections_manager.h"

namespace meow {
namespace models {
namespace db {

class EntitiesTreeModel : public QAbstractItemModel
{
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

private:
    meow::db::ConnectionsManager * _dbConnectionsManager;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_ENTITIESTREEMODEL_H
