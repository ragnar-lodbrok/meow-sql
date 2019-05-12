#include "query_data_editor.h"
#include "query_data.h"
#include "query.h"
#include "editable_grid_data.h"
#include "entity/table_entity.h"

namespace meow {
namespace db {

bool QueryDataEditor::applyModificationsInDB(QueryData * data)
{
    Q_ASSERT(data->query());

    if (!data->isModified()) return false;

    QStringList updateDataList;
    QStringList insertColumnsList;
    QStringList insertValuesList;
    Connection * connection = data->query()->connection();

    EditableGridData * editableData = data->query()->editableData();
    Q_ASSERT(editableData);

    bool isInsert = editableData->editableRow()->isInserted;

    int editedRowNumber = editableData->editableRow()->rowNumber;

    std::size_t columnCount = data->query()->columnCount();

    for (size_t c = 0; c < columnCount; ++c) {
        const QString & oldValue
                = editableData->notModifiedDataAt(editedRowNumber, c);
        const QString & newValue
                = editableData->dataAt(editedRowNumber, c);

        if ( (oldValue == newValue)
             && (oldValue.isNull() == newValue.isNull()) ) {
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
                .arg(connection->limitOnePostfix(false));

        connection->query(updateSQL);
        // TODO check rows affected
        return true;
    } else if (!insertColumnsList.isEmpty()) {
        QString insertSQL = QString("INSERT INTO %1 (%2) VALUES (%3)")
                .arg(db::quotedFullName(data->query()->entity()))
                .arg(insertColumnsList.join(", "))
                .arg(insertValuesList.join(", "));

        connection->query(insertSQL);

        editableData->editableRow()->isInserted = false;

        Entity * entity = data->query()->entity();
        if (entity && entity->type() == Entity::Type::Table) {
            TableEntity * table = static_cast<TableEntity *>(entity);
            const auto & columns = table->structure()->columns();
            for (int c = 0; c < columns.size(); ++c) {
                TableColumn * column = columns[c];
                if (column->defaultType() == ColumnDefaultType::AutoInc) {
                    int intVal = editableData->editableRow()->data[c].toInt();
                    editableData->editableRow()->data[c]
                        = QString::number(intVal);
                    if (intVal == 0) {
                        editableData->editableRow()->data[c]
                            = connection->getCell("SELECT LAST_INSERT_ID()");
                    }
                }
            }
        }

        return true;
    }

    return false;
}

void QueryDataEditor::deleteCurrentRow(QueryData * data)
{
    // Listening: Vader - Piesc I Stal
    Q_ASSERT(data->query());

    Connection * connection = data->query()->connection();

    QString deleteSQL = QString("DELETE FROM %1 WHERE %2 %3")
            .arg(db::quotedFullName(data->query()->entity()))
            .arg(data->whereForCurRow(true))
            .arg(connection->limitOnePostfix(false));

    connection->query(deleteSQL);

    // TODO check rows affected
}

} // namespace db
} // namespace meow
