#include "table_indexes_model.h"

namespace meow {
namespace models {
namespace db {

TableIndexesModel::TableIndexesModel(QObject * parent)
    : QAbstractItemModel(parent)
{

}

Qt::ItemFlags TableIndexesModel::flags(const QModelIndex &index) const // override
{
    if (!index.isValid()) {
        return 0;
    }

    return QAbstractItemModel::flags(index);
}

QModelIndex TableIndexesModel::index(int row,
                             int column,
                             const QModelIndex &parent) const // override
{

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return QModelIndex();
}

QModelIndex TableIndexesModel::parent(const QModelIndex &index) const // override
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    return QModelIndex();
}

QVariant TableIndexesModel::data(const QModelIndex &index, int role) const // override
{

    Q_UNUSED(role);

    if (!index.isValid()) {
        return QVariant();
    }

    return QVariant();
}

int TableIndexesModel::rowCount(const QModelIndex &parent) const // override
{
    if (parent.column() > 0) {
        return 0;
    }

    return 0;
}

int TableIndexesModel::columnCount(const QModelIndex &parent) const // override
{
    Q_UNUSED(parent);

    return static_cast<int>(Columns::Count);
}

QVariant TableIndexesModel::headerData(int section,
                    Qt::Orientation orientation,
                    int role) const
{

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Name:
            return QString(tr("Name"));

        case Columns::Type:
            return QString(tr("Type / Length"));

        case Columns::Algorithm:
            return QString(tr("Algorithm"));

        default:
            break;
        }
    }

    return QVariant();

}

int TableIndexesModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Name:
        return 300;
    case Columns::Type:
        return 120;
    case Columns::Algorithm:
        return 100;
    default:
        return 80;
    }
}

} // namespace db
} // namespace models
} // namespace meow
