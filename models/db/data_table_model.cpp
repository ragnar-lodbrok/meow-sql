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
     _dbEntity(nullptr),
     _queryData()
{

}

int DataTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

int DataTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
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

    }

    return QVariant();
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    //if (index.row() >= entitiesCount()) {
    //    return QVariant();
    //}


    if (role == Qt::DisplayRole) {



    } else if (role == Qt::DecorationRole) {


    }


    return QVariant();
}

void DataTableModel::setEntity(meow::db::Entity * tableOrViewEntity)
{
    // Listening: As I Lay Dying - Defender
    _dbEntity = tableOrViewEntity;

    if (_dbEntity == nullptr) {
        return;
    }

    meow::db::QueryDataFetcher * queryDataFetcher =
        _dbEntity->connection()->createQueryDataFetcher();

    std::shared_ptr<meow::db::QueryDataFetcher> fetcher(queryDataFetcher);

    meow::db::QueryCriteria queryCritera;
    queryCritera.quotedDbAndTableName = _dbEntity->name(); // TODO

    queryDataFetcher->run(&queryCritera, &_queryData);

}


} // namespace db
} // namespace models
} // namespace meow
