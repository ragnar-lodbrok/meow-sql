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

bool TableEntityComparator::commentDiffers() const
{
    QString commentPrev = _prev ? _prev->structure()->comment() : QString();
    QString commentCurr = _curr ? _curr->structure()->comment() : QString();

    return commentPrev != commentCurr;
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

QList<TableIndex *> TableEntityComparator::removedIndices() const
{
    QList<TableIndex *> removedIndices;

    TableStructure * newStructure = _curr->structure();
    TableStructure * oldStructure = _prev->structure();

    const QList<TableIndex *> & oldIndices = oldStructure->indicies();

    for (const auto & oldIndex : oldIndices) {
        TableIndex * newIndex = newStructure->indexById(oldIndex->id());
        if (newIndex == nullptr) {
            removedIndices.append(oldIndex);
        }
    }

    return removedIndices;
}

QList<TableIndexStatus> TableEntityComparator::currIndicesWithStatus() const
{
    QList<TableIndexStatus> indices;
    TableStructure * newStructure = _curr->structure();
    TableStructure * oldStructure = _prev->structure();

    const QList<TableIndex *> & newIndices = newStructure->indicies();

    for (auto & newIndex : newIndices) {

        TableIndex * oldIndex = oldStructure->indexById(newIndex->id());

        TableIndexStatus status;

        status.oldIndex = oldIndex; // nullptr if added
        status.newIndex = newIndex;

        if (oldIndex) {
            status.modified = oldIndex->dataDiffers(newIndex);
        } else {
            status.added = true;
        }

        indices.append(status);
    }

    return indices;
}

} // namespace db
} // namespace meow
