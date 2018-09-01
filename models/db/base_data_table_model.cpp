#include "base_data_table_model.h"
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace models {
namespace db {

BaseDataTableModel::BaseDataTableModel(meow::db::QueryData * queryData,
                                         QObject *parent)
    : QAbstractTableModel(parent),
      _queryData(queryData)
{

}

int BaseDataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _queryData->columnCount();
}

int BaseDataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _queryData->rowCount();
}

Qt::ItemFlags BaseDataTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant BaseDataTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        return _queryData->columnName(section);
    }

    return QVariant();
}

QVariant BaseDataTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {


    case Qt::EditRole: // TODO: NULL?
        // TODO: EnsureFullRow
        _queryData->prepareEditing(); // Let's do it here
    case Qt::DisplayRole:        
        return _queryData->rawDataAt(index.row(), index.column());

    case Qt::ForegroundRole: {
        auto textSettings = meow::app()->settings()->textSettings();
        auto dataType = _queryData->columnDataTypeCategory(index.column());
        if (dataType != meow::db::DataTypeCategoryIndex::None) {
            bool isNull = _queryData->isNullAt(index.row(), index.column());
            if (isNull == false) {
                return textSettings->colorForDataType(dataType);
            } else {
                return textSettings->colorForDataTypeNULL(dataType);
            }
        }
    }

    default:
        return QVariant();
    }

}

} // namespace db
} // namespace models
} // namespace meow
