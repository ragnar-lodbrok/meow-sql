#include "query_data_editor.h"
#include "query_data.h"
#include "query.h"
#include "editable_grid_data.h"

namespace meow {
namespace db {

void QueryDataEditor::applyModificationsInDB(QueryData * data)
{
    Q_ASSERT(data->query());

    if (!data->isModified()) return;

    EditableGridData * editableData = data->query()->editableData();
    Q_ASSERT(editableData);

    std::size_t editedRowNumber = editableData->editableRow()->rowNumber;

    std::size_t columnCount = data->query()->columnCount();

    for (size_t c = 0; c < columnCount; ++c) {
        const QString & oldValue
                = editableData->notModifiedDataAt(editedRowNumber, c);
        const QString & newValue
                = editableData->dataAt(editedRowNumber, c);

        if (oldValue != newValue) {
            // TODO
        }
    }
}

} // namespace db
} // namespace meow
