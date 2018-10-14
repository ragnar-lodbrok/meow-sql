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
    explicit DataTab(QWidget *parent = 0);

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
    void validateToolBarState();

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    Q_SLOT void onDataSetNULLAction(bool checked);

    void applyModifications();
    void discardModifications();

    void commitTableEditor();
    void discardTableEditor();

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
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // CENTRAL_RIGHT_DATA_TAB_H
