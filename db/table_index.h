#ifndef DB_TABLE_INDEX_H
#define DB_TABLE_INDEX_H

#include <QString>
#include <QStringList>
#include <QList>

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
const QStringList tableIndexClassNames();

TableIndexType strToTableIndexType(const QString & str);
QString tableIndexTypeToStr(TableIndexType type);
const QStringList tableIndexTypeNames();

class TableEntity;
class TableColumn;

// Represents table key/index
class TableIndex
{
public:

    class Column
    {
    public:
        Column(TableIndex * index, unsigned columnId)
            : _index(index),
              _columnId(columnId)
        {

        }
        QString name() const;
        unsigned id() const { return _columnId; }

    private:
        TableIndex * _index;
        unsigned _columnId;
    };

    explicit TableIndex(TableEntity * table);

    void setTable(TableEntity * table);
    TableEntity * table() const { return _table; }

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
    QList<Column> & columns() { return _columns; }
    const QStringList columnNames() const {
        QStringList names;
        for (const auto & column : _columns) {
            names.append(column.name());
        }
        return names;
    }
    //QStringList & subParts() { return _subParts; } // TODO

    bool hasColumn(const QString & name) {
        for (const auto & column : _columns) {
            if (column.name() == name) {
                return true;
            }
        }
        return false;
    }

    int addColumn(const QString & name);

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

    bool removeColumn(const QString & name) {
        return removeColumnIndex(columnIndex(name));
    }

    int columnIndex(const QString & name) {
        int i = 0;
        for (const auto & column : _columns) {
            if (column.name() == name) {
                return i;
            }
            ++i;
        }
        return -1;
    }

    bool canMoveColumnUp(int index) const {
        return index >= 1 && index < _columns.size();
    }

    bool canMoveColumnDown(int index) const {
        return index >=0 && index < (_columns.size() - 1);
    }

    operator QString() const;

    TableIndex * deepCopy(TableEntity * table);

private:
    TableEntity * _table;
    QString _name;
    TableIndexClass _class; // H: IndexType
    TableIndexType _type; // H: Algorithm
    QList<Column> _columns;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_INDEX_H
