#include "table_index.h"
#include <QMap>

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

TableIndex::TableIndex()
    :_class(TableIndexClass::None),
     _type(TableIndexType::None)
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
        str += " columns:" + _columns.join(',');
    }

    return str;
}

} // namespace db
} // namespace meow
