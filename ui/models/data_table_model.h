#ifndef DATA_TABLE_MODEL_H
#define DATA_TABLE_MODEL_H

#include <QObject>
#include "query_data_sort_filter_proxy_model.h"
#include "base_data_table_model.h"
#include "db/common.h"
#include "ui/delegates/edit_query_data_delegate.h"

// Main Window
//   Central Right Widget
//     Data Tab
//       Table Model

namespace meow {

namespace db {
class TableColumn;
}

namespace ui {
namespace models {

class DataTableModel : public BaseDataTableModel,
                       public delegates::IItemDelegateConfig
{
    Q_OBJECT
public:
    explicit DataTableModel(QObject *parent = nullptr);
    virtual ~DataTableModel() override;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;
    virtual delegates::EditorType editorType(
            const QModelIndex &index) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    void setEntity(meow::db::Entity * tableOrViewEntity, bool loadData = true);
    meow::db::Entity * entity() const { return _dbEntity; }

    void removeData();
    void loadData(bool force = false);
    void refresh();
    void invalidateData();

    void setNoRowsCountLimit();
    void incRowsCountForOneStep(bool reset = false);
    bool isLimited() const;
    bool isFiltered() const;
    bool allDataLoaded() const;

    void applyWhereFilter(const QString & whereFilter);
    void resetWhereFilter();

    QString rowCountStats() const;

    bool isEditable() const;
    bool isEditing();
    bool isModified();

    void applyModifications(int rowToApply = -1);
    void discardModifications();

    void setCurrentRowNumber(int row);

    bool deleteRowInDB(int row);

    int insertEmptyRow();
    int duplicateCurrentRowWithoutKeys();
    int duplicateCurrentRowWithKeys();
    int insertNewRow(bool duplicateCurrent = false, bool withKeys = false);

    QAbstractItemModel * createSortFilterModel();

    void setFilterPattern(const QString & pattern, bool regexp);
    QString filterPattern() const;
    bool filterPatternIsRegexp() const { return _filterPatternIsRegexp; }

    int filterMatchedRowCount() const;

    QList<db::TableColumn *> selectedTableColumns();

    Q_SIGNAL void editingStarted();
    Q_SIGNAL void dataRefreshed();

    void changeColumnSort(int columnIndex);
    bool isColumnSorted(int columnIndex) const;
    Qt::SortOrder columnSortOrder(int columnIndex) const;
    void resetAllColumnsSort();
    void applyColumnSort();

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

    QModelIndex createIndexForRow(int row) {
        return createIndex(row, 0);
    }

private:

    QueryDataSortFilterProxyModel * _sortFilterModel;
    QString _filterPattern;
    bool _filterPatternIsRegexp;

    bool _entityChangedProcessed;
    meow::db::Entity * _dbEntity;
    meow::db::ulonglong _wantedRowsCount;
    QString _whereFilter;

    struct SortColumn
    {
        int columnIndex = -1;
        Qt::SortOrder sortOrder = Qt::AscendingOrder;
    };

    std::vector<SortColumn> _columnsSort;
};


} // namespace models
} // namespace ui
} // namespace meow

#endif // DATA_TABLE_MODEL_H
