#include "table_columns_model.h"

namespace meow {
namespace models {
namespace db {

TableColumnsModel::TableColumnsModel(QObject * parent) : QAbstractTableModel(parent)
{

}

int TableColumnsModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return (int)Columns::Count;
}

int TableColumnsModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return 5; // Test
}

Qt::ItemFlags TableColumnsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TableColumnsModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Number:
            return QString(tr("#"));

        case Columns::Name:
            return QString(tr("Name"));

        case Columns::DataType:
            return QString(tr("Datatype"));

        case Columns::Length:
            return QString(tr("Length/Set"));

        case Columns::Unsigned:
            return QString(tr("Unsigned"));

        case Columns::AllowNull:
            return QString(tr("Allow NULL"));

        case Columns::Zerofill:
            return QString(tr("Zerofill"));

        case Columns::Default:
            return QString(tr("Default"));

        case Columns::Comment:
            return QString(tr("Comment"));

        case Columns::Collation:
            return QString(tr("Collation"));

        case Columns::Expression:
            return QString(tr("Expression"));

        case Columns::Virtuality:
            return QString(tr("Virtuality"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant TableColumnsModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    // TODO

    return QVariant();
}

} // namespace db
} // namespace models
} // namespace meow
