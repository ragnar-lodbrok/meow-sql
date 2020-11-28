#ifndef CENTRAL_RIGHT_DATA_TAB_H
#define CENTRAL_RIGHT_DATA_TAB_H

#include <QtWidgets>
#include "models/db/data_table_model.h"
#include "models/delegates/edit_query_data_delegate.h"
#include "ui/main_window/central_right/global_data_filter_interface.h"

namespace meow {
namespace ui {

class EditableDataTableView;

namespace main_window {
namespace central_right {

class DataTab : public QWidget, public IGlobalDataFilter
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

    const models::db::DataTableModel * model() const {
        return &_model;
    }

private:

    void onLoadData();

    Q_SLOT void actionAllRows(bool checked);
    Q_SLOT void actionNextRows(bool checked);

    void createDataTable();
    void createTopPanel();
    void createShowToolBar();
    void createDataToolBar();

    void refreshDataLabelText();

    void connectRowChanged();
    void disconnectRowChanged();

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    Q_SLOT void currentChanged(const QModelIndex &current,
                               const QModelIndex &previous);

    Q_SLOT void onDataSetNULLAction(bool checked);
    Q_SLOT void onDataRefreshAction(bool checked);

    Q_SLOT void onDataPostChanges(bool checked);
    Q_SLOT void onDataCancelChanges(bool checked);
    Q_SLOT void onDataDelete(bool checked);
    Q_SLOT void onDataInsert(bool checked);

    Q_SIGNAL void changeRowSelection(const QModelIndex &index);
    Q_SLOT void onChangeRowSelectionRequest(const QModelIndex &index);

    bool applyModifications(int rowToApply = -1);
    void discardModifications();
    void deleteSelectedRows();
    void insertEmptyRow();

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
    QToolBar * _showToolBar;
    QToolBar * _dataToolBar;
    QAction * _nextRowsAction;
    QAction * _showAllRowsAction;
    // bottom:
    EditableDataTableView  * _dataTable;

    models::db::DataTableModel _model;

    models::delegates::EditQueryDataDelegate * _defaultTableDelegate;
    models::delegates::EditTextQueryDataDelegate * _textColumnTableDelegate;

    bool _skipApplyModifications;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // CENTRAL_RIGHT_DATA_TAB_H
