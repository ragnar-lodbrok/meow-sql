#ifndef MEOW_UI_EDITABLE_QUERY_DATA_TABLE_VIEW_H
#define MEOW_UI_EDITABLE_QUERY_DATA_TABLE_VIEW_H

#include "table_view.h"

namespace meow {
namespace ui {

namespace models {
class BaseDataTableModel;
}

class EditableQueryDataTableView : public TableView
{
    Q_OBJECT
public:
    EditableQueryDataTableView(models::BaseDataTableModel * model,
                               QWidget * parent = nullptr);
protected:
    virtual void contextMenuEvent(QContextMenuEvent * event) override;
private:
    models::BaseDataTableModel * _model;
};

} // namespace ui
} // namespace meow

#endif // MEOW_UI_EDITABLE_QUERY_DATA_TABLE_VIEW_H
