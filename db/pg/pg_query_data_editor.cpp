#include "pg_query_data_editor.h"
#include "db/query_data.h"
#include "db/query.h"
#include "db/editable_grid_data.h"
#include "db/entity/table_entity.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

void PGQueryDataEditor::insert(
        QueryData * data,
        const QStringList & columns,
        const QStringList & values)
{
    Connection * connection = data->query()->connection();

    EditableGridData * editableData = data->query()->editableData();

    QString insertSQL = QString("INSERT INTO %1 (%2) VALUES (%3) RETURNING *")
        .arg(db::quotedFullName(data->query()->entity()))
        .arg(columns.join(", "))
        .arg(values.join(", "));

    // insert and get the whole new row, PG is cool
    QStringList newRowData = connection->getRow(insertSQL);

    editableData->editableRow()->isInserted = false;

    EditableGridDataRow * row = data->query()->editableData()->editableRow();

    if (newRowData.size() != row->data.size()) {
        meowLogC(Log::Category::Error) << "Failed to load inserted row";
        //Q_ASSERT(false); // OK, e.g. table structure changed on server
        return;
    }

    // we get SEQUENCE values as well
    row->data = newRowData; // assume the same column order

    _modificationsLoaded = true; // avoid extra select
}

} // namespace db
} // namespace meow

