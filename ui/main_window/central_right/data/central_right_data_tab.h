#ifndef CENTRAL_RIGHT_DATA_TAB_H
#define CENTRAL_RIGHT_DATA_TAB_H

#include <QtWidgets>
#include "models/db/data_table_model.h"

namespace meow {
namespace ui {

class EditableDataTableView;

namespace main_window {
namespace central_right {

class DataTab : public QWidget
{

public:
    explicit DataTab(QWidget *parent = nullptr);

    void setDBEntity(db::Entity * tableOrViewEntity, bool loadData = true);

    void loadData();

private:

    Q_SLOT void actionAllRows(bool checked);
    Q_SLOT void actionNextRows(bool checked);

    void createDataTable();
    void createTopPanel();
    void createShowToolBar();
    void createDataToolBar();

    void refreshDataLabelText();

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    Q_SLOT void currentChanged(const QModelIndex &current,
                               const QModelIndex &previous);

    Q_SLOT void onDataSetNULLAction(bool checked);

    Q_SLOT void onDataPostChanges(bool checked);
    Q_SLOT void onDataCancelChanges(bool checked);
    Q_SLOT void onDataDelete(bool checked);
    Q_SLOT void onDataInsert(bool checked);

    void applyModifications();
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

    bool _skipApplyModifications;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // CENTRAL_RIGHT_DATA_TAB_H
