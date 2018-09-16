#include "query_data_editor.h"
#include "query_data.h"
#include "query.h"
#include "editable_grid_data.h"
#include "entity/entity.h"

namespace meow {
namespace db {

void QueryDataEditor::applyModificationsInDB(QueryData * data)
{
    Q_ASSERT(data->query());

    if (!data->isModified()) return;

    QStringList updateDataList;
    Connection * connection = data->query()->connection();

    EditableGridData * editableData = data->query()->editableData();
    Q_ASSERT(editableData);

    std::size_t editedRowNumber = editableData->editableRow()->rowNumber;

    std::size_t columnCount = data->query()->columnCount();

    for (size_t c = 0; c < columnCount; ++c) {
        const QString & oldValue
                = editableData->notModifiedDataAt(editedRowNumber, c);
        const QString & newValue
                = editableData->dataAt(editedRowNumber, c);

        if (oldValue == newValue) {
            continue; // not modified
        }

        QString valInDB;
        if (newValue.isNull()) {
            valInDB = "NULL";
        } else {
            // TODO: bit/spatial/temporal preprocessing
            valInDB = connection->escapeString(newValue);
        }

        QString columnName = data->query()->column(c).orgName;
        updateDataList
            << connection->quoteIdentifier(columnName) + "=" + valInDB;
    }

    if (!updateDataList.isEmpty()) {
        QString updateSQL = QString("UPDATE %1 SET %2 WHERE %3 %4")
                .arg(db::quotedFullName(data->query()->entity()))
                .arg(updateDataList.join(", "))
                .arg(data->whereForCurRow(true))
                .arg(connection->limitOnePostfix());;

        connection->query(updateSQL);
        // TODO check rows affected
    }
}

} // namespace db
} // namespace meow
