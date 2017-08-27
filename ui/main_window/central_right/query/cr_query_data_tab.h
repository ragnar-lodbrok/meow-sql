#ifndef UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H
#define UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H

#include <QtWidgets>
#include "models/db/base_data_table_model.h"

namespace meow {

namespace db {
    class QueryData;
}

namespace ui {
namespace main_window {
namespace central_right {

class QueryDataTab : public QWidget
{
public:
    explicit QueryDataTab(db::QueryData * queryData, QWidget *parent = 0);
    virtual ~QueryDataTab();
private:
    models::db::BaseDataTableModel _model;
    QTableView  * _dataTable;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H
