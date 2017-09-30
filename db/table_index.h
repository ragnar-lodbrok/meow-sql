#ifndef DB_TABLE_INDEX_H
#define DB_TABLE_INDEX_H

#include <QString>
#include <QStringList>

// https://dev.mysql.com/doc/refman/5.7/en/create-index.html
// TODO: index_option, algorithm_option, lock_option

namespace meow {
namespace db {

enum class TableIndexClass {
    None,
    PrimaryKey,
    Key,
    Unique,
    FullText,
    Spatial
};

enum class TableIndexType {
    None,
    BTREE,
    HASH,
    RTREE
};

// Represents table key/index
class TableIndex
{
public:
    TableIndex();
private:
    QString _name;
    TableIndexClass _class; // H: IndexType
    TableIndexType _type; // H: Algorithm
    QStringList _columns;
    QStringList _subParts; // ?
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_INDEX_H
