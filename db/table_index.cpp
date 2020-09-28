#include "table_index.h"
#include <QMap>
#include <QDebug>
#include "table_column.h"
#include "entity/table_entity.h"

namespace meow {
namespace db {

TableIndexClass strToTableIndexClass(const QString & str)
{
    static QMap<QString, TableIndexClass> map = {
        { "PRIMARY",  TableIndexClass::PrimaryKey },
        { "KEY",      TableIndexClass::Key },
        { "UNIQUE",   TableIndexClass::Unique },
        { "FULLTEXT", TableIndexClass::FullText },
        { "SPATIAL",  TableIndexClass::Spatial },
    };

    QString strUp = str.toUpper();
    return map.value(strUp, TableIndexClass::None);
}

QString tableIndexClassToStr(TableIndexClass cls)
{
    switch (cls) {
    case TableIndexClass::PrimaryKey:
        return "PRIMARY";
    case TableIndexClass::Key:
        return "KEY";
    case TableIndexClass::Unique:
        return "UNIQUE";
    case TableIndexClass::FullText:
        return "FULLTEXT";
    case TableIndexClass::Spatial:
        return "SPATIAL";
    default:
        return "NONE";
    }
}

const QStringList tableIndexClassNames()
{
    static QStringList list = {
        "PRIMARY",
        "KEY",
        "UNIQUE",
        "FULLTEXT",
        "SPATIAL"
    };
    return list;
}

TableIndexType strToTableIndexType(const QString & str)
{
    static QMap<QString, TableIndexType> map = {
        { "BTREE",  TableIndexType::BTREE },
        { "HASH",   TableIndexType::HASH },
        { "RTREE",  TableIndexType::RTREE }
    };

    QString strUp = str.toUpper();
    return map.value(strUp, TableIndexType::None);
}

QString tableIndexTypeToStr(TableIndexType type)
{
    switch (type) {
    case TableIndexType::BTREE:
        return "BTREE";
    case TableIndexType::HASH:
        return "HASH";
    case TableIndexType::RTREE:
        return "RTREE";
    default:
        return "NONE";
    }
}

const QStringList tableIndexTypeNames()
{
    static QStringList list = {
        "BTREE",
        "HASH",
        "RTREE"
    };
    return list;
}

QString TableIndex::Column::name() const {
    TableColumn * column = _index->table()->structure()->columnById(_columnId);
    return column ? column->name() : QString();
}

TableIndex::TableIndex(TableEntity * table)
    :_table(table),
     _class(TableIndexClass::None),
     _type(TableIndexType::None),
     _id(0)
{

}

TableIndex::operator QString() const
{
    QString str = QString("name:%1 class:%2")
            .arg(_name)
            .arg(tableIndexClassToStr(_class));

    if (_type != TableIndexType::None) {
        str += " type:" + tableIndexTypeToStr(_type);
    }

    if (!_columns.isEmpty()) {
        str += " columns:" + columnNames().join(',');
    }

    return str;
}

QString TableIndex::name() const
{
    if (_class == TableIndexClass::PrimaryKey) {
        return "PRIMARY KEY";
    }
    return _name;
}

int TableIndex::addColumn(const QString & name)
{
    TableStructure * structure = _table->structure();
    TableColumn * column = structure->columnByName(name);
    if (column) {
        _columns.append(Column(this, column->id()));
        if (isPrimaryKey()) {
            column->setAllowNull(false);
        }
        emit structure->columnRelationChangedForIndex(column, this);
        return _columns.size();
    }
    return -1;
}

bool TableIndex::replaceColumn(int index, const QString & name)
{
    if (!isValidColumnIndex(index)) return false;

    TableStructure * structure = _table->structure();
    TableColumn * column = structure->columnByName(name);

    if (column) {
        unsigned oldColumnId = _columns.at(index).id();
        TableColumn * oldColumn = structure->columnById(oldColumnId);

        _columns.replace(index, Column(this, column->id()));

        emit structure->columnRelationChangedForIndex(column, this);
        emit structure->columnRelationChangedForIndex(oldColumn, this);
        return true;
    }
    return false;
}

bool TableIndex::removeColumnIndex(int index) {
    if (isValidColumnIndex(index)) {

        TableStructure * structure = _table->structure();
        unsigned columnId = _columns.at(index).id();
        TableColumn * column = structure->columnById(columnId);

        _columns.removeAt(index);

        emit structure->columnRelationChangedForIndex(column, this);

        return true;
    }
    return false;
}

void TableIndex::setTable(TableEntity * table)
{
    _table = table;
}

bool TableIndex::hasColumnsWithAllowNull() const
{
    for (const auto & column : _columns) {
        TableColumn * tableColumn = _table->structure()->columnById(column.id());
        if (tableColumn && tableColumn->isAllowNull()) {
            return true;
        }
    }

    return false;
}

TableIndex * TableIndex::deepCopy(TableEntity * table)
{
    TableIndex * copy = new TableIndex(*this);
    copy->_table = table;
    copy->_columns.clear();

    for (auto & column : _columns) {
        copy->_columns.append(Column(copy, column.id()));
    }

    return copy;
}

bool TableIndex::dataDiffers(const TableIndex * other) const
{
     if (this == other) return false;

     //if (_table != other->_table) return true; // TODO
     if (_name != other->_name) return true;
     if (_class != other->_class) return true;
     if (_type != other->_type) return true;

     auto isNotSubset = [=](
         const QList<Column> & columns1,
         const QList<Column> & columns2) -> bool {
         for (const auto & column1 : columns1) {
             bool found = false;
             for (const auto & column2 : columns2) {
                 if (column1.id() == column2.id()) {
                     found = true;
                     break;
                 }
             }
             if (!found) {
                 return true;
             }
         }
         return false;
     };

     if (isNotSubset( _columns, other->_columns) ) return true;
     if (isNotSubset( other->_columns, _columns) ) return true;

     return false;
}

} // namespace db
} // namespace meow
