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

enum class TableIndexType { // == H: Algorithm
    None,
    BTREE,
    HASH,
    RTREE
};

TableIndexClass strToTableIndexClass(const QString & str);
QString tableIndexClassToStr(TableIndexClass cls);

TableIndexType strToTableIndexType(const QString & str);
QString tableIndexTypeToStr(TableIndexType type);

// Represents table key/index
class TableIndex
{
public:

    TableIndex();

    QString name() const;
    void setName(const QString & name) { _name = name; }

    TableIndexClass classType() const { return _class; }
    QString classTypeStr() const {
        return tableIndexClassToStr(_class);
    }
    void setClassType(TableIndexClass cls) { _class = cls; }
    void setClassType(const QString & clsStr) {
        _class = strToTableIndexClass(clsStr);
    }

    TableIndexType indexType() const { return _type; }
    QString indexTypeStr() const {
        return tableIndexTypeToStr(_type);
    }
    void setIndexType(TableIndexType type) { _type = type; }
    void setIndexType(const QString & typeStr) {
        _type = strToTableIndexType(typeStr);
    }

    int columnsCount() const { return _columns.size(); }
    QStringList & columns() { return _columns; }
    //QStringList & subParts() { return _subParts; }

    bool hasColumn(const QString & name) {
        return _columns.contains(name);
    }

    int addColumn(const QString & name) {
       _columns.append(name);
       return _columns.size();
    }

    bool isValidColumnIndex(int index) const {
        return index >=0 && index < _columns.size();
    }

    bool removeColumnIndex(int index) {
        if (isValidColumnIndex(index)) {
            _columns.removeAt(index);
            return true;
        }
        return false;
    }

    bool canMoveColumnUp(int index) const {
        return index >= 1 && index < _columns.size();
    }

    bool canMoveColumnDown(int index) const {
        return index >=0 && index < (_columns.size() - 1);
    }

    operator QString() const;

private:
    QString _name;
    TableIndexClass _class; // H: IndexType
    TableIndexType _type; // H: Algorithm
    QStringList _columns;
    // TODO: separate class for Column
    //QStringList _subParts; // ?
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_INDEX_H
