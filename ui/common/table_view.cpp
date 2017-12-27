#include "table_view.h"
#include "app.h"

namespace meow {
namespace ui {

static const int colMargin = 10;
static const int minColWidth = 90;

int TableView::sizeHintForColumn(int column) const
{
    // TODO: respect sizes set manually or by user
    int parentHint = QTableView::sizeHintForColumn(column);
    if (parentHint > 0) {
        if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
            return qMax(parentHint + colMargin, minColWidth);
        }
    }
    return parentHint;
}

} // namespace ui
} // namespace meow
