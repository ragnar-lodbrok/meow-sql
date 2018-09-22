#ifndef MEOW_UI_EDITABLE_DATA_TABLE_VIEW_H
#define MEOW_UI_EDITABLE_DATA_TABLE_VIEW_H

#include "table_view.h"

namespace meow {
namespace ui {

class EditableDataTableView : public TableView
{

protected:
    virtual void contextMenuEvent(QContextMenuEvent * event) override;
};

} // namespace ui
} // namespace meow

#endif // MEOW_UI_EDITABLE_DATA_TABLE_VIEW_H
