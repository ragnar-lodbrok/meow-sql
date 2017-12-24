#include "table_foreign_keys_model.h"
#include "db/entity/table_entity.h"
#include <QIcon>

namespace meow {
namespace models {
namespace forms {

TableForeignKeysModel::TableForeignKeysModel(QObject * parent)
    : QAbstractTableModel(parent),
      _table(nullptr)
{

}

void TableForeignKeysModel::setTable(meow::db::TableEntity * table)
{
    removeData();
    _table = table;
    insertData();
}

int TableForeignKeysModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int TableForeignKeysModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (_table && _table->structure()) {
        return _table->structure()->foreighKeys().size();
    }
    return 0;
}

Qt::ItemFlags TableForeignKeysModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TableForeignKeysModel::headerData(
                                    int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {

        case Columns::Name:
            return QString(tr("Key name"));

        case Columns::Columns:
            return QString(tr("Columns"));

        case Columns::ReferenceTable:
            return QString(tr("Reference table"));

        case Columns::ForeignColumns:
            return QString(tr("Foreign columns"));

        case Columns::OnUpdate:
            return QString(tr("On UPDATE"));

        case Columns::OnDelete:
            return QString(tr("On DELETE"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant TableForeignKeysModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {

    case Qt::DisplayRole:
        return textDataAt(index.row(), index.column());

    case Qt::DecorationRole:
        if (static_cast<Columns>(index.column()) == Columns::Name) {
            // TODO: move to Settings::icons
            static const QIcon icon = QIcon(":/icons/table_relationship.png");
            return icon;
        }

    default:
        break;
    }

    return QVariant();
}

int TableForeignKeysModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {

    case Columns::Name:
        return 220;

    case Columns::ReferenceTable:
        return 170;

    default:
       return 130;
    }
}

QString TableForeignKeysModel::textDataAt(int row, int col) const
{
    meow::db::ForeignKey * fKey = _table->structure()->foreighKeys().at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Name:
        return fKey->name();

    case Columns::Columns:
        return fKey->columns().join(',');

    case Columns::ReferenceTable:
        return fKey->referenceTable();

    case Columns::ForeignColumns:
        return fKey->referenceColumns().join(',');

    case Columns::OnUpdate:
        return referenceOptionToStr(fKey->onUpdate());

    case Columns::OnDelete:
        return referenceOptionToStr(fKey->onDelete());

    default:
       return QString();
    }
}

void TableForeignKeysModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}
void TableForeignKeysModel::insertData()
{
    if (rowCount() > 0) {
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

} // namespace forms
} // namespace models
} // namespace meow
