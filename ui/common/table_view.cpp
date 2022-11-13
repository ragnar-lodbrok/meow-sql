#include "table_view.h"
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace ui {

static const int colMargin = 20; // extra space for editors
static const int minColWidth = 90;
static const int maxColWidth = 500;

TableView::TableView(QWidget * parent)
    : QTableView(parent)
{

}

int TableView::sizeHintForColumn(int column) const
{
    // TODO: respect sizes set manually or by user
    int parentHint = QTableView::sizeHintForColumn(column);
    if (parentHint > 0) {
        if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
            return qMin(qMax(parentHint + colMargin, minColWidth), maxColWidth);
        }
    }
    return parentHint;
}

} // namespace ui
} // namespace meow
