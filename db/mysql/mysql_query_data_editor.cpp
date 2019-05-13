#include "mysql_query_data_editor.h"
#include "db/query_data.h"
#include "db/query.h"
#include "db/editable_grid_data.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace db {

void MySQLQueryDataEditor::insert(
        QueryData * data,
        const QStringList & columns,
        const QStringList & values)
{
    QueryDataEditor::insert(data, columns, values);

    EditableGridData * editableData = data->query()->editableData();

    Connection * connection = data->query()->connection();

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
}

} // namespace db
} // namespace meow

