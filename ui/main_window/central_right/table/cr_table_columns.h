#ifndef UI_CENTRAL_RIGHT_TABLE_COLUMNS_H
#define UI_CENTRAL_RIGHT_TABLE_COLUMNS_H

#include <QtWidgets>
#include "ui/models/table_columns_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TableTab;
class TableColumnsTools;

// Intent: Table columns list/editor
class TableColumns : public QWidget
{
public:
    explicit TableColumns(TableTab * parent);

    void setTable(db::TableEntity * table);

    Q_SLOT void actionAddColumn(bool checked);
    Q_SLOT void actionRemoveColumn(bool checked);
    Q_SLOT void actionMoveColumnUp(bool checked);
    Q_SLOT void actionMoveColumnDown(bool checked);

    void validateControls();

    const models::TableColumnsModel * model() const { return &_model; }

private:

    Q_SLOT void currentRowChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    void createWidgets();
    void scrollToCurrent();

    TableColumnsTools * _tools;
    QTableView  * _columnsTable;

    models::TableColumnsModel _model;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_COLUMNS_H
