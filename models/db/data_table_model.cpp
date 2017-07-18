#include "data_table_model.h"
#include "db/query_data_fetcher.h"
#include "db/connection.h"
#include "db/common.h"
#include "db/query_criteria.h"
#include <QDebug>

namespace meow {
namespace models {
namespace db {

DataTableModel::DataTableModel(QObject *parent)
    :QAbstractTableModel(parent),
      _dataLoaded(false),
     _dbEntity(nullptr),
     _queryData()
{

}

int DataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _queryData.columnCount();
}

int DataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _queryData.rowCount();
}

Qt::ItemFlags DataTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant DataTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        return _queryData.columnName(section);
    }

    return QVariant();
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= rowCount()) {
        return QVariant();
    }


    if (role == Qt::DisplayRole) {
        return _queryData.rawDataAt(index.row(), index.column());
    } else if (role == Qt::DecorationRole) {

    }


    return QVariant();
}

void DataTableModel::setEntity(meow::db::Entity * tableOrViewEntity, bool loadData)
{
    removeData();

    // Listening: As I Lay Dying - Defender
    _dbEntity = tableOrViewEntity;
    _dataLoaded = false; // not loaded since last entity change

    if (loadData) {
        this->loadData(true);
    }
}

void DataTableModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }

    if (columnCount()) {
        beginRemoveColumns(QModelIndex(), 0, columnCount()-1);
        endRemoveColumns();
    }
}

void DataTableModel::loadData(bool force)
{
    if (_dbEntity == nullptr) {
        return;
    }

    if (force == false && _dataLoaded) {
        return;
    }

    meow::db::QueryDataFetcher * queryDataFetcher =
        _dbEntity->connection()->createQueryDataFetcher();

    std::shared_ptr<meow::db::QueryDataFetcher> fetcher(queryDataFetcher);

    meow::db::QueryCriteria queryCritera;
    queryCritera.quotedDbAndTableName = meow::db::quotedFullName(_dbEntity);

    queryDataFetcher->run(&queryCritera, &_queryData);

    _dataLoaded = true;

    if (rowCount() && columnCount()) {

        beginInsertColumns(QModelIndex(), 0, columnCount()-1);
        endInsertColumns();

        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

void DataTableModel::refresh()
{
    removeData();
    loadData(true);
}

} // namespace db
} // namespace models
} // namespace meow
