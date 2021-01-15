#include "editable_data_table_view.h"
#include "app/app.h"
#include "db/connection.h"
#include "helpers/formatting.h"

#include <QtWidgets>

namespace meow {
namespace ui {

void EditableDataTableView::contextMenuEvent(QContextMenuEvent * event)
{

    QMenu menu(this);

    QMenu * insertValueSubMenu = menu.addMenu( // owns result
        QIcon(":/icons/calendar_view_day.png"),
        tr("Insert value"));

    // TODO check for supportsEditingTablesData()
    insertValueSubMenu->addAction( meow::app()->actions()->dataSetNULL() );

    meow::db::Connection * currentConnection
            = meow::app()->dbConnectionsManager()->activeConnection();

    QDateTime currentTimestamp = currentConnection->currentServerTimestamp();
    if (currentTimestamp.isValid()) {

        uint64_t unixTimestamp = currentTimestamp.toMSecsSinceEpoch() / 1000;

        insertValueSubMenu->addSeparator();

        QList<QPair<QString, QString> > datetimeValueLabels = {
            {helpers::formatDateTime(currentTimestamp), tr("Date and time: %1")},
            {helpers::formatDate(currentTimestamp), tr("Date: %1")},
            {helpers::formatTime(currentTimestamp), tr("Time: %1")},
            {helpers::formatYear(currentTimestamp), tr("Year: %1")},
            {QString::number(unixTimestamp), tr("UNIX Timestamp: %1")}
        };

        for (const auto & valueAndLabel : datetimeValueLabels) {

            QAction * timestampAction
                    = new QAction(
                        QIcon(":/icons/calendar_view_day.png"),
                        valueAndLabel.second.arg(valueAndLabel.first),
                         &menu);
            timestampAction->setData(valueAndLabel.first);

            connect(timestampAction,
                    &QAction::triggered,
                    [=](){
                        QModelIndex curIndex = selectionModel()->currentIndex();
                        model()->setData(curIndex, timestampAction->data());
                    }
            );

            insertValueSubMenu->addAction(timestampAction);
        }
    }

    menu.addSeparator();

    menu.addAction(meow::app()->actions()->dataRefresh());

    menu.exec(event->globalPos());
}

} // namespace ui
} // namespace meow
