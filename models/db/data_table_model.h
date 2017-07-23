#ifndef DATA_TABLE_MODEL_H
#define DATA_TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "db/entity/entity.h"
#include "db/query_data.h"
#include "db/common.h"

// Main Window
//   Central Right Widget
//     Data Tab
//       Table Model

namespace meow {
namespace models {
namespace db {

class DataTableModel : public QAbstractTableModel
{
public:
    explicit DataTableModel(QObject *parent = nullptr);
    virtual ~DataTableModel() {}

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setEntity(meow::db::Entity * tableOrViewEntity, bool loadData = true);
    void removeData();
    void loadData(bool force = false);
    void refresh();

    void setNoRowsCountLimit();
    void incRowsCountForOneStep(bool reset = false);
    bool isLimited() const;
    bool allDataLoaded() const;

    QString rowCountStats() const;

private:
    bool _entityChangedProcessed;
    meow::db::Entity * _dbEntity;
    meow::db::QueryData _queryData;
    meow::db::ulonglong _wantedRowsCount;
};


} // namespace db
} // namespace models
} // namespace meow

#endif // DATA_TABLE_MODEL_H
