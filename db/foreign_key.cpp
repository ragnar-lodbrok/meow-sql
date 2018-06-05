#include "foreign_key.h"
#include "table_column.h"
#include "table_structure.h"
#include "entity/table_entity.h"
#include <QMap>
#include <QDebug>

namespace meow {
namespace db {


ForeignKey::ForeignKey(TableEntity * table)
    :_table(table),
     _id(0),
     _isCustomName(true),
     _onUpdate(ReferenceOption::Restrict),
     _onDelete(ReferenceOption::Restrict)
{
    // For an ON DELETE or ON UPDATE that is not specified,
    // the default action is always RESTRICT.
}

ForeignKey::operator QString() const
{
    QString str =
    QString("CONSTRAINT `%1` FOREIGN KEY (%2) REFERENCES `%3` (%4)")
            .arg(_name)
            .arg(columnNames().join(','))
            .arg(_referenceTable)
            .arg(_referenceColumns.join(','));

    if (_onDelete != ReferenceOption::Restrict) {
        str += " ON DELETE " + referenceOptionToStr(_onDelete);
    }
    if (_onUpdate != ReferenceOption::Restrict) {
        str += " ON UPDATE " + referenceOptionToStr(_onUpdate);
    }

    return str;
}

void ForeignKey::setOnUpdate(const QString & opt) {
    _onUpdate = strToReferenceOption(opt);
}

void ForeignKey::setOnDelete(const QString & opt) {
    _onDelete = strToReferenceOption(opt);
}

ForeignKey::ReferenceOption strToReferenceOption(
    const QString & str)
{

    static QMap<QString, ForeignKey::ReferenceOption> map = {
        { "NO ACTION",   ForeignKey::ReferenceOption::NoAction },
        { "RESTRICT",    ForeignKey::ReferenceOption::Restrict },
        { "CASCADE",     ForeignKey::ReferenceOption::Cascade },
        { "SET NULL",    ForeignKey::ReferenceOption::SetNull },
        { "SET DEFAULT", ForeignKey::ReferenceOption::SetDefault }
    };

    QString strUp = str.toUpper();
    return map.value(strUp, ForeignKey::ReferenceOption::Restrict);
}

QString referenceOptionToStr(ForeignKey::ReferenceOption opt)
{
    switch (opt) {
    case ForeignKey::ReferenceOption::Restrict:
        return "RESTRICT";
    case ForeignKey::ReferenceOption::Cascade:
        return "CASCADE";
    case ForeignKey::ReferenceOption::SetNull:
        return "SET NULL";
    case ForeignKey::ReferenceOption::SetDefault:
        return "SET DEFAULT";
    default:
        return "NO ACTION";
    }
}

QStringList ForeignKey::columnNames() const
{
    QStringList names;
    for (const auto & column : _columns) {
        names.append(column->name());
    }

    return names;
}

void ForeignKey::setColumns(const QStringList & columnNames)
{
    _columns.clear();
    for (const auto & columnName : columnNames) {
        TableColumn * column = _table->structure()->columnByName(columnName);
        if (column) {
            _columns.append(column);
        }
    }
}

ForeignKey * ForeignKey::deepCopy(TableStructure * structure)
{
    ForeignKey * copy = new ForeignKey(*this);
    copy->_columns.clear();
    QList<TableColumn *> oldColumns = this->_columns;

    for (auto & oldColumn : oldColumns) {
        TableColumn * newColumn = structure->columnByName(
            oldColumn->name());
        if (newColumn) {
            copy->_columns.append(newColumn);
        }
    }

    return copy;
}

bool ForeignKey::removeColumnByName(const QString & columnName)
{
    int indexToRemove = -1;
    int i = 0;
    QString name = _columns.isEmpty() ? QString() : columnName.toLower();
    for (const auto & column : _columns) {
        if (column->name().toLower() == name) {
            indexToRemove = i;
            break;
        }
        ++i;
    }
    if (indexToRemove != -1) {
        _columns.removeAt(indexToRemove);
        return true;
    }
    return false;
}

bool ForeignKey::dataDiffers(const ForeignKey * other) const
{
    if (this == other) return false;

    if (_name != other->_name) return true;
    if (_referenceTable != other->_referenceTable) return true;
    if (_onUpdate != other->_onUpdate) return true;
    if (_onDelete != other->_onDelete) return true;

    auto isNotSubsetColumns = [=](
        const QList<TableColumn *> columns1,
        const QList<TableColumn *> columns2) -> bool {
        for (const auto & column1 : columns1) {
            bool found = false;
            for (const auto & column2 : columns2) {
                if (column1->id() == column2->id()) {
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

    if (isNotSubsetColumns( _columns, other->_columns) )
        return true;
    if (isNotSubsetColumns( other->_columns, _columns) )
        return true;

    auto isNotSubsetStrList = [=](
        const QStringList & columns1,
        const QStringList & columns2) -> bool {
        for (const auto & column1 : columns1) {
            bool found = false;
            for (const auto & column2 : columns2) {
                if (column1.toLower() == column2.toLower()) {
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

    if (isNotSubsetStrList( _referenceColumns, other->_referenceColumns) )
        return true;
    if (isNotSubsetStrList( other->_referenceColumns, _referenceColumns) )
        return true;

    return false;
}

QStringList referenceOptionNames()
{
    QStringList names;

    int i = static_cast<int>(ForeignKey::ReferenceOption::NoAction);
    int count = static_cast<int>(ForeignKey::ReferenceOption::Count);
    for (; i < count; ++i) {
        names << referenceOptionToStr(
                    static_cast<ForeignKey::ReferenceOption>(i)
                 );
    }

    return names;
}

} // namespace db
} // namespace meow
