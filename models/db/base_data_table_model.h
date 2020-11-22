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
public:
    explicit BaseDataTableModel(
        meow::db::QueryData * queryData,
        QObject * parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    meow::db::QueryData * queryData() { return _queryData; }

    meow::db::DataTypeCategoryIndex typeCategoryForColumn(int column) const {
        return _queryData->columnDataTypeCategory(column);
    }

    // We need to handle row/col count from outside bc Qt models require
    // to make something like that:
    //   beginInsertRows();
    //   fetchData(); // add actual rows here
    //   endInsertRows();
    // But we already changed queryData + plus query may fail
    void setRowCount(int newRowCount); // set to -1 to take from queryData()
    void setColumnCount(int newColumnCount);


private:
    meow::db::QueryData * _queryData;
    int _rowCount;
    int _colCount;
};


} // namespace db
} // namespace models
} // namespace meow

#endif // BASE_DATA_TABLE_MODEL_H
