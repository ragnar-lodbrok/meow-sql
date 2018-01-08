#ifndef DB_TABLE_ENTITY_COMPARATOR_H
#define DB_TABLE_ENTITY_COMPARATOR_H

namespace meow {
namespace db {

class TableEntity;

class TableEntityComparator
{
public:
    TableEntityComparator();

    void setPrevTable(TableEntity * prev) { _prev = prev; }
    void setCurrTable(TableEntity * curr) { _curr = curr; }

    bool nameDiffers() const;

private:
    TableEntity * _prev;
    TableEntity * _curr;

};

} // namespace db
} // namespace meow

#endif // DB_TABLE_ENTITY_COMPARATOR_H
