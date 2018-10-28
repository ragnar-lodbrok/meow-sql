#include "editable_data_table_view.h"
#include "app/app.h"

#include <QtWidgets>

namespace meow {
namespace ui {

void EditableDataTableView::contextMenuEvent(QContextMenuEvent * event)
{

    QMenu menu(this);

    QMenu * insertValueSubMenu = menu.addMenu( // owns result
        QIcon(":/icons/calendar_view_day.png"),
        tr("Insert value"));

    insertValueSubMenu->addAction( meow::app()->actions()->dataSetNULL() );

    menu.exec(event->globalPos());
}

} // namespace ui
} // namespace meow
