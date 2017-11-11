#ifndef UI_CENTRAL_RIGHT_TABLE_COLUMNS_H
#define UI_CENTRAL_RIGHT_TABLE_COLUMNS_H

#include <QtWidgets>
#include "models/db/table_columns_model.h"

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
    TableColumns(TableTab * parent);

    void setTable(db::TableEntity * table);

private:

    void createWidgets();

    TableColumnsTools * _tools;
    QTableView  * _columnsTable;
    models::db::TableColumnsModel _model;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_COLUMNS_H
