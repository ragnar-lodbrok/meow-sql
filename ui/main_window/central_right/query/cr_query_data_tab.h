#ifndef UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H
#define UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H

#include <QtWidgets>
#include "ui/models/base_data_table_model.h"
#include "ui/common/editable_query_data_table_view.h"
#include "db/query_data.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class QueryDataTab : public QWidget
{
public:
    explicit QueryDataTab(db::QueryDataPtr queryData, QWidget *parent = 0);
    virtual ~QueryDataTab();

    void setFilterPattern(const QString & pattern, bool regexp);

    int filterMatchedRowCount() const;
    int totalRowCount() const;

    void onDataExportAction();

private:

    Q_SLOT void onDataTableHeaderClicked(int index);

    models::BaseDataTableModel _model;
    EditableQueryDataTableView  * _dataTable;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_QUERY_DATA_TAB_H
