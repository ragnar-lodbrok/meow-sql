#ifndef DATA_TABLE_MODEL_H
#define DATA_TABLE_MODEL_H

#include <QObject>
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

// Intent: model for Table or View
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

    QList<db::TableColumn *> selectedTableColumns();

    Q_SIGNAL void editingStarted();
    Q_SIGNAL void dataRefreshed();

    void changeColumnSort(int columnIndex);
    bool isColumnSorted(int columnIndex) const;
    Qt::SortOrder columnSortOrder(int columnIndex) const;
    void resetAllColumnsSort();
    void applyColumnSort();

private:

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
