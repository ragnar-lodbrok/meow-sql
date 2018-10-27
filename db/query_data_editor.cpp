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
    QStringList insertColumnsList;
    QStringList insertValuesList;
    Connection * connection = data->query()->connection();

    EditableGridData * editableData = data->query()->editableData();
    Q_ASSERT(editableData);

    bool isInsert = editableData->editableRow()->isInserted;

    if (isInsert) {
        return; // TODO
    }

    int editedRowNumber = editableData->editableRow()->rowNumber;

    std::size_t columnCount = data->query()->columnCount();

    for (size_t c = 0; c < columnCount; ++c) {
        const QString & oldValue
                = editableData->notModifiedDataAt(editedRowNumber, c);
        const QString & newValue
                = editableData->dataAt(editedRowNumber, c);

        if ( (oldValue == newValue) && (oldValue.isNull() == newValue.isNull()) ) {
            continue; // not modified
        }

        QString valInDB;
        if (newValue.isNull()) {
            valInDB = "NULL";
        } else {
            // TODO: bit/spatial/temporal preprocessing
            valInDB = connection->escapeString(newValue);
        }

        QString columnName = connection->quoteIdentifier(
                    data->query()->column(c).orgName);

        if (isInsert) {
            insertColumnsList << columnName;
            insertValuesList << valInDB;
        } else {
            updateDataList << columnName + "=" + valInDB;
        }
    }

    if (!updateDataList.isEmpty()) {
        QString updateSQL = QString("UPDATE %1 SET %2 WHERE %3 %4")
                .arg(db::quotedFullName(data->query()->entity()))
                .arg(updateDataList.join(", "))
                .arg(data->whereForCurRow(true))
                .arg(connection->limitOnePostfix());

        connection->query(updateSQL);
        // TODO check rows affected
    } else if (!insertColumnsList.isEmpty()) {
        // Connection.Query('INSERT INTO '+QuotedDbAndTableName+' ('+sqlInsertColumns+') VALUES ('+sqlInsertValues+')');
        QString insertSQL = QString("INSERT INTO %1 (%2) VALUES (%3)")
                .arg(db::quotedFullName(data->query()->entity()))
                .arg(insertColumnsList.join(", "))
                .arg(updateDataList.join(", "));
        // TODO: SELECT LAST_INSERT_ID()
    }
}

void QueryDataEditor::deleteCurrentRow(QueryData * data)
{
    // Listening: Vader - Piesc I Stal
    Q_ASSERT(data->query());

    Connection * connection = data->query()->connection();

    QString deleteSQL = QString("DELETE FROM %1 WHERE %2 %3")
            .arg(db::quotedFullName(data->query()->entity()))
            .arg(data->whereForCurRow(true))
            .arg(connection->limitOnePostfix());

    connection->query(deleteSQL);

    // TODO check rows affected
}

} // namespace db
} // namespace meow
