#ifndef DATA_TABLE_MODEL_H
#define DATA_TABLE_MODEL_H

#include <QObject>
#include "base_data_table_model.h"
#include "db/common.h"

// Main Window
//   Central Right Widget
//     Data Tab
//       Table Model

namespace meow {
namespace models {
namespace db {

class DataTableModel : public BaseDataTableModel
{
public:
    explicit DataTableModel(QObject *parent = nullptr);
    virtual ~DataTableModel();

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;

    void setEntity(meow::db::Entity * tableOrViewEntity, bool loadData = true);
    void removeData();
    void loadData(bool force = false);
    void refresh();

    void setNoRowsCountLimit();
    void incRowsCountForOneStep(bool reset = false);
    bool isLimited() const;
    bool allDataLoaded() const;

    QString rowCountStats() const;

    void applyModifications();
    void discardModifications();

    void setCurrentRowNumber(int row);

private:

    bool _entityChangedProcessed;
    meow::db::Entity * _dbEntity;
    meow::db::ulonglong _wantedRowsCount;
};


} // namespace db
} // namespace models
} // namespace meow

#endif // DATA_TABLE_MODEL_H
