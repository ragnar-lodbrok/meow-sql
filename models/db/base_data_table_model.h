#ifndef BASE_DATA_TABLE_MODEL_H
#define BASE_DATA_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "db/query_data.h"

namespace meow {
namespace models {
namespace db {

// Intent: base model to wrap QueryData (table/query data)
class BaseDataTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BaseDataTableModel(
        meow::db::QueryData * queryData,
        QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    meow::db::QueryData * queryData() { return _queryData; }

private:
    meow::db::QueryData * _queryData;
};


} // namespace db
} // namespace models
} // namespace meow

#endif // BASE_DATA_TABLE_MODEL_H
