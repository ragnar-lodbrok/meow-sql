#ifndef BASE_DATA_TABLE_MODEL_H
#define BASE_DATA_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "db/query_data.h"
#include "query_data_sort_filter_proxy_model.h"

namespace meow {
namespace ui {
namespace models {

// Intent: base model to wrap QueryData (table/query data)
class BaseDataTableModel : public QAbstractTableModel
{
public:
    explicit BaseDataTableModel(
        meow::db::QueryDataPtr queryData,
        QObject * parent = nullptr);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    virtual int rowCount(
            const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(
            const QModelIndex &parent = QModelIndex()) const override;

    meow::db::QueryData * queryData() { return _queryData.get(); }

    meow::db::DataTypeCategoryIndex typeCategoryForColumn(int column) const {
        return _queryData->columnDataTypeCategory(column);
    }

    bool columnHasForeignKey(int column) const {
        return _queryData->columnHasForeignKey(column);
    }

    bool columnIsAutoIncrement(int column) const {
        return _queryData->columnIsAutoIncrement(column);
    }

    meow::db::DataTypePtr dataTypeForColumn(int column) const {
        return _queryData->dataTypeForColumn(column);
    }  
    meow::db::Connection * connection() const {
        if (_queryData->query()) {
            return _queryData->query()->connection();
        }
        return nullptr;
    }

    bool isNullAt(int row, int column) const;

    // We need to handle row/col count from outside bc Qt models require
    // to make something like that:
    //   beginInsertRows();
    //   fetchData(); // add actual rows here
    //   endInsertRows();
    // But we already changed queryData + plus query may fail
    void setRowCount(int newRowCount); // set to -1 to take from queryData()
    void setColumnCount(int newColumnCount);

    QAbstractItemModel * createSortFilterModel();

    void setFilterPattern(const QString & pattern, bool regexp);
    QString filterPattern() const;
    bool filterPatternIsRegexp() const { return _filterPatternIsRegexp; }

    int filterMatchedRowCount() const;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const {
        if (_sortFilterModel) {
            return _sortFilterModel->mapToSource(proxyIndex);
        } else {
            return proxyIndex;
        }
    }

    QModelIndexList mapToSource(const QModelIndexList &proxyIndexList) const {
        if (_sortFilterModel) {
            QModelIndexList result = proxyIndexList;
            for (auto & index : result) {
                index = _sortFilterModel->mapToSource(index);
            }
            return result;
        } else {
            return proxyIndexList;
        }
    }

    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const {
        if (_sortFilterModel) {
            return _sortFilterModel->mapFromSource(sourceIndex);
        } else {
            return sourceIndex;
        }
    }

    QModelIndexList mapFromSource(const QModelIndexList &sourceIndexList) const {
        if (_sortFilterModel) {
            QModelIndexList result = sourceIndexList;
            for (auto & index : result) {
                index = _sortFilterModel->mapFromSource(index);
            }
            return result;
        } else {
            return sourceIndexList;
        }
    }

    QModelIndex createIndexForRow(int row) const {
        return createIndex(row, 0);
    }

    QModelIndex createIndexForRowCol(int row, int col) const {
        return createIndex(row, col);
    }

private:
    meow::db::QueryDataPtr _queryData;
    int _rowCount;
    int _colCount;

    QueryDataSortFilterProxyModel * _sortFilterModel;
    QString _filterPattern;
    bool _filterPatternIsRegexp;

};


} // namespace models
} // namespace ui
} // namespace meow

#endif // BASE_DATA_TABLE_MODEL_H
