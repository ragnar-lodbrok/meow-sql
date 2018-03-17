#ifndef DB_TABLE_ENTITY_COMPARATOR_H
#define DB_TABLE_ENTITY_COMPARATOR_H

#include <QList>

namespace meow {
namespace db {

class TableEntity;
class TableColumn;

struct TableColumnPair {
    TableColumn * oldCol = nullptr;
    TableColumn * newCol = nullptr;
};

struct TableColumnStatus
{
    TableColumnPair columns;
    bool added = false;
    bool modified = false;
};

class TableEntityComparator
{
public:
    TableEntityComparator();

    void setPrevTable(TableEntity * prev) { _prev = prev; }
    void setCurrTable(TableEntity * curr) { _curr = curr; }

    bool nameDiffers() const;
    bool commentDiffers() const;

    // return pairs of columns that (data) were modified (not removed, inserted)
    QList<TableColumnPair> modifiedColumns() const;

    QList<TableColumn *> removedColumns() const;

    // returns list of all current columns with statuses
    QList<TableColumnStatus> currColumnsWithStatus() const;

private:

    TableEntity * _prev;
    TableEntity * _curr;

};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENTITY_COMPARATOR_H
