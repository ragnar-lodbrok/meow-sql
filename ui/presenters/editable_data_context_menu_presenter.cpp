#include "editable_data_context_menu_presenter.h"

#include <QObject>
#include "app/app.h"
#include "db/connection.h"
#include "ui/models/data_table_model.h"
#include "helpers/formatting.h"

namespace meow {
namespace ui {
namespace presenters {

EditableDataContextMenuPresenter::EditableDataContextMenuPresenter(
        models::BaseDataTableModel * model)
    : _model(model)
{

}

bool EditableDataContextMenuPresenter::supportsInsertValue() const
{
    if (isQueryModel()) return false; // not yet supported

    meow::db::Connection * currentConnection
            = meow::app()->dbConnectionsManager()->activeConnection();

    if (!currentConnection) return false;

    return currentConnection->features()->supportsEditingTablesData();
}

QAction * EditableDataContextMenuPresenter::setNullAction() const
{
    return meow::app()->actions()->dataSetNULL();
}

QAction * EditableDataContextMenuPresenter::refreshDataAction() const
{
    if (isQueryModel()) return nullptr; // not yet supported
    return meow::app()->actions()->dataRefresh();
}

std::vector<QAction *>
EditableDataContextMenuPresenter::setDateTimeActions() const
{
    std::vector<QAction *> dateTimeActions;

    meow::db::Connection * currentConnection
            = meow::app()->dbConnectionsManager()->activeConnection();

    QDateTime currentTimestamp = currentConnection->currentServerTimestamp();
    if (currentTimestamp.isValid()) {

        uint64_t unixTimestamp = currentTimestamp.toMSecsSinceEpoch() / 1000;

        QList<QPair<QString, QString> > datetimeValueLabels;

        datetimeValueLabels.push_back({helpers::formatDateTime(currentTimestamp),
                                       QObject::tr("Date and time: %1")});

        datetimeValueLabels.push_back({helpers::formatDate(currentTimestamp),
                                       QObject::tr("Date: %1")});

        datetimeValueLabels.push_back({helpers::formatTime(currentTimestamp),
                                       QObject::tr("Time: %1")});

        datetimeValueLabels.push_back({helpers::formatYear(currentTimestamp),
                                       QObject::tr("Year: %1")});

        datetimeValueLabels.push_back({QString::number(unixTimestamp),
                                       QObject::tr("UNIX Timestamp: %1")});

        dateTimeActions.reserve(datetimeValueLabels.size());

        for (const auto & valueAndLabel : datetimeValueLabels) {

            QAction * timestampAction
                    = new QAction(
                        QIcon(":/icons/calendar_view_day.png"),
                        valueAndLabel.second.arg(valueAndLabel.first),
                        nullptr);
            timestampAction->setData(valueAndLabel.first);

            dateTimeActions.push_back(timestampAction);
        }
    }

    return dateTimeActions;
}

std::vector<QAction *> EditableDataContextMenuPresenter::editRowActions() const
{
    if (isQueryModel()) return {}; // not yet supported

    std::vector<QAction *> actions = {
        meow::app()->actions()->dataInsertRow(),
        meow::app()->actions()->dataDuplicateRowWithoutKeys(),
        meow::app()->actions()->dataDuplicateRowWithKeys(),
        meow::app()->actions()->dataPostChanges(),
        meow::app()->actions()->dataCancelChanges(),
        meow::app()->actions()->dataDeleteRows()
    };

    return actions;
}

QAction * EditableDataContextMenuPresenter::resetDataSortAction() const
{
    if (isQueryModel()) return nullptr; // not yet supported
    return meow::app()->actions()->dataResetSort();
}

QAction * EditableDataContextMenuPresenter::exportDataAction() const
{
    return meow::app()->actions()->dataExport();
}

bool EditableDataContextMenuPresenter::isTableModel() const
{
    return dynamic_cast<ui::models::DataTableModel *>(_model) != nullptr;
}

bool EditableDataContextMenuPresenter::isQueryModel() const
{
    return !isTableModel();
}

} // namespace presenters
} // namespace ui
} // namespace meow
