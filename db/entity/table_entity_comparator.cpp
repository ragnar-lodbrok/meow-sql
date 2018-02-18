#include "table_entity_comparator.h"
#include "table_entity.h"

namespace meow {
namespace db {

TableEntityComparator::TableEntityComparator()
    :_prev(nullptr),
     _curr(nullptr)
{

}

bool TableEntityComparator::nameDiffers() const
{
    // Listening: Be´lakor - Roots to Sever

    QString namePrev = _prev ? _prev->name() : QString();
    QString nameCurr = _curr ? _curr->name() : QString();

    return namePrev != nameCurr;
}

QList<TableColumnPair> TableEntityComparator::modifiedColumns() const
{
    QList<TableColumnPair> modifiedColumns;
    TableStructure * newStructure = _curr->structure();
    TableStructure * oldStructure = _prev->structure();

    const QList<TableColumn *> & newColumns = newStructure->columns();

    for (auto & newColumn : newColumns) {
        TableColumn * oldColumn = oldStructure->columnById(newColumn->id());
        if (oldColumn && oldColumn->dataDiffers(newColumn)) {
            TableColumnPair pair;
            pair.oldCol = oldColumn;
            pair.newCol = newColumn;
            modifiedColumns.append(pair);
        }
    }

    return modifiedColumns;
}

QList<TableColumn *> TableEntityComparator::removedColumns() const
{
    QList<TableColumn *> removedColumns;

    TableStructure * newStructure = _curr->structure();
    TableStructure * oldStructure = _prev->structure();

    const QList<TableColumn *> & oldColumns = oldStructure->columns();

    for (auto & oldColumn : oldColumns) {
        TableColumn * newColumn = newStructure->columnById(oldColumn->id());
        if (newColumn == nullptr) {
            removedColumns.append(oldColumn);
        }
    }

    return removedColumns;
}

QList<TableColumnStatus> TableEntityComparator::currColumnsWithStatus() const
{
    QList<TableColumnStatus> columns;
    TableStructure * newStructure = _curr->structure();
    TableStructure * oldStructure = _prev->structure();

    const QList<TableColumn *> & newColumns = newStructure->columns();

    for (auto & newColumn : newColumns) {

        TableColumn * oldColumn = oldStructure->columnById(newColumn->id());

        TableColumnStatus status;

        status.columns.oldCol = oldColumn; // nullptr if added
        status.columns.newCol = newColumn;

        if (oldColumn) {
            bool dataDiffers = oldColumn->dataDiffers(newColumn);
            if (dataDiffers) {
                status.modified = true;
            } else {
                // TODO: check moved
            }
        } else {
            status.added = true;
        }

        columns.append(status);
    }

    return columns;
}

} // namespace db
} // namespace meow
