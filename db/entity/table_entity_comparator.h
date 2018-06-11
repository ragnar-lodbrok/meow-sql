#ifndef DB_TABLE_ENTITY_COMPARATOR_H
#define DB_TABLE_ENTITY_COMPARATOR_H

#include <QList>

namespace meow {
namespace db {

class TableEntity;
class TableColumn;
class TableIndex;
class ForeignKey;

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

struct TableIndexStatus
{
    TableIndex * oldIndex = nullptr;
    TableIndex * newIndex = nullptr;
    bool added = false;
    bool modified = false;
};

struct ForeignKeyPair {
    ForeignKey * oldFKey = nullptr;
    ForeignKey * newFkey = nullptr;
};

struct ForeignKeyStatus
{
    ForeignKeyPair keys;
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
    bool collateDiffers() const;
    bool engineDiffers() const;
    bool rowFormatDiffers() const;
    bool checksumDiffers() const;
    bool autoIncrementDiffers() const;
    bool avgRowLenDiffers() const;
    bool maxRowsDiffers() const;

    // return columns that (data) were modified (not removed, not inserted)
    QList<TableColumnPair> modifiedColumns() const;

    QList<TableColumn *> removedColumns() const;

    // returns list of all current columns with statuses
    QList<TableColumnStatus> currColumnsWithStatus() const;

    QList<TableIndex *> removedIndices() const;

    QList<TableIndexStatus> currIndicesWithStatus() const;

    QList<ForeignKey *> removedForeignKeys() const;

    QList<ForeignKey *> addedForeignKeys() const;
    
    // return keys that (data) were modified (not removed, not inserted)
    QList<ForeignKeyPair> modifiedForeignKeys() const;

private:

    TableEntity * _prev;
    TableEntity * _curr;

};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENTITY_COMPARATOR_H
