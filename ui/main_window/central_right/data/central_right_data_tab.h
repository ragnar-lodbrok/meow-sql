#ifndef CENTRAL_RIGHT_DATA_TAB_H
#define CENTRAL_RIGHT_DATA_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/models/data_table_model.h"
#include "ui/delegates/edit_query_data_delegate.h"
#include "ui/main_window/central_right/global_data_filter_interface.h"
#include "ui/main_window/central_right/data/cr_data_filter_widget.h"

namespace meow {
namespace ui {

class EditableDataTableView;

namespace main_window {
namespace central_right {

class DataTab : public BaseRootTab, public IGlobalDataFilter
{
    Q_OBJECT
public:
    explicit DataTab(QWidget *parent = nullptr);

    void setDBEntity(db::Entity * tableOrViewEntity, bool loadData = true);

    void refresh();

    void loadData();

    void invalidateData();

    virtual void setFilterPattern(const QString & filter,
                                  bool regexp = false) override;
    virtual QString filterPattern() const override;
    virtual bool filterPatternIsRegexp() const override;

    virtual int totalRowCount() const override;
    virtual int filterMatchedRowCount() const override;

    const models::DataTableModel * model() const {
        return &_model;
    }

private:

    void onLoadData();

    Q_SLOT void onActionAllRows();
    Q_SLOT void onActionNextRows();
    Q_SLOT void onActionShowFilter(bool checked);

    void createDataTable();
    void createTopPanel();
    void createDataButtonsToolBar();
    void createDataActionsToolBar();

    void refreshDataLabelText();

    void connectRowChanged();
    void disconnectRowChanged();

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    Q_SLOT void currentChanged(const QModelIndex &current,
                               const QModelIndex &previous);

    Q_SLOT void onDataSetNULLAction(bool checked);
    Q_SLOT void onDataRefreshAction(bool checked);
    Q_SLOT void onDataResetSortAction();
    Q_SLOT void onDataExportAction();

    Q_SLOT void onDataPostChanges(bool checked);
    Q_SLOT void onDataCancelChanges(bool checked);
    Q_SLOT void onDataDeleteRows();
    Q_SLOT void onDataInsertRow();
    Q_SLOT void onDataDuplicateRowWithoutKeys();
    Q_SLOT void onDataDuplicateRowWithKeys();
    Q_SLOT void onDataRefreshed();

    Q_SIGNAL void changeRowSelection(const QModelIndex &index);
    Q_SLOT void onChangeRowSelectionRequest(const QModelIndex &index);

    Q_SLOT void onDataTableHeaderClicked(int index);

    bool applyModifications(int rowToApply = -1);
    void discardModifications();
    void deleteSelectedRows();
    void insertEmptyRow();
    void duplicateCurrentRowWithoutKeys();
    void duplicateCurrentRowWithKeys();
    void insertNewRow(bool duplicateCurrent = false, bool withKeys = false);

    void commitTableEditor();
    void discardTableEditor();

    void validateShowToolBarState();
    void validateDataToolBarState();
    void validateDataDeleteActionState();
    Q_SLOT void validateControls();

    void errorDialog(const QString & message);

    QAbstractItemDelegate * currentItemDelegate() const;
    
    QModelIndex currentIndexMapped() const;

    QVBoxLayout * _mainLayout;
    // top panel:
    QHBoxLayout * _topLayout;
    QLabel * _dataLabel;
    QToolBar * _dataButtonsToolBar;
    QToolBar * _dataActionsToolBar;
    QAction * _nextRowsAction;
    QAction * _showAllRowsAction;
    QAction * _showFilterPanelAction;
    DataFilterWidget * _dataFilter;
    // bottom:
    EditableDataTableView  * _dataTable;

    models::DataTableModel _model;

    delegates::EditQueryDataDelegate * _defaultTableDelegate;
    delegates::FormatTextQueryDataDelegate * _textColumnTableDelegate;

    bool _skipApplyModifications;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // CENTRAL_RIGHT_DATA_TAB_H
