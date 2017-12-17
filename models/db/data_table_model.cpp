#include "data_table_model.h"
#include "db/query_data_fetcher.h"
#include "db/connection.h"
#include "db/common.h"
#include "db/query_criteria.h"
#include <QDebug>
#include "helpers/formatting.h"
#include "db/entity/table_entity.h"
#include <QColor>
#include "app.h"

namespace meow {
namespace models {
namespace db {

DataTableModel::DataTableModel(QObject *parent)
    :BaseDataTableModel(new meow::db::QueryData(), parent),
      _entityChangedProcessed(false),
      _dbEntity(nullptr),
      _wantedRowsCount(meow::db::DATA_MAX_ROWS)
{

}

DataTableModel::~DataTableModel()
{
    delete queryData(); // oh shit
}

void DataTableModel::setEntity(meow::db::Entity * tableOrViewEntity, bool loadData)
{
    removeData();

    // Listening: As I Lay Dying - Defender
    _dbEntity = tableOrViewEntity;
    _entityChangedProcessed = false;

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

    if (force == false && _entityChangedProcessed) {
        return;
    }

    meow::db::QueryDataFetcher * queryDataFetcher =
        _dbEntity->connection()->createQueryDataFetcher();

    std::shared_ptr<meow::db::QueryDataFetcher> fetcher(queryDataFetcher);

    meow::db::ulonglong offset = 0;
    int prevColCount = 0;
    int prevRowCount = 0;

    if (_entityChangedProcessed) { // load from the same table/view
        offset = rowCount();
        prevColCount = columnCount();
        prevRowCount = rowCount();
    }

    meow::db::QueryCriteria queryCritera;
    queryCritera.quotedDbAndTableName = meow::db::quotedFullName(_dbEntity);
    queryCritera.limit = _wantedRowsCount - offset;
    queryCritera.offset = offset;

    queryDataFetcher->run(&queryCritera, queryData());

    _entityChangedProcessed = true;

    if (columnCount() > prevColCount) {
        beginInsertColumns(QModelIndex(), prevColCount, columnCount()-1);
        endInsertColumns();
    }

    if (rowCount() > prevRowCount) {
        beginInsertRows(QModelIndex(), prevRowCount, rowCount()-1);
        endInsertRows();
    }
}

void DataTableModel::refresh()
{
    removeData();
    loadData(true);
}

QString DataTableModel::rowCountStats() const
{
    if (_dbEntity == nullptr) {
        return QString();
    }
    QString result = QString("");
    QString databaseName = meow::db::databaseName(_dbEntity);
    if (databaseName.length()) {
        result = databaseName + ".";
    }
    result += _dbEntity->name();

    if (_dbEntity->type() == meow::db::Entity::Type::Table) {

        meow::db::TableEntity * table =
            static_cast<meow::db::TableEntity *>(_dbEntity);

        meow::db::ulonglong rowsCount = 0;
        if (_entityChangedProcessed && !isLimited()) {
            rowsCount = rowCount();
        } else {
            rowsCount = table->rowsCount(true);// TODO: rm extra query
        }

        result += ": " + meow::helpers::formatNumber(rowsCount) + " ";
        result += QObject::tr("rows total");

        if (table->engineStr() == "InnoDB") {
            result += " (" + QObject::tr("approximately") + ")";
        }        
        if (isLimited()) {
            result += ", " + QObject::tr("limited to");
            result += " " + meow::helpers::formatNumber(rowCount());
        }

        // TODO: where
    }

    return result;
}

void DataTableModel::setNoRowsCountLimit()
{
    _wantedRowsCount = meow::db::DATA_MAX_ROWS;
}

void DataTableModel::incRowsCountForOneStep(bool reset)
{
    if (reset) {
        _wantedRowsCount = 0;
    }
    _wantedRowsCount += meow::db::DATA_ROWS_PER_STEP;
    if (_wantedRowsCount > meow::db::DATA_MAX_ROWS) {
        _wantedRowsCount = meow::db::DATA_MAX_ROWS;
    }
}

bool DataTableModel::isLimited() const
{
    return _wantedRowsCount <= (meow::db::ulonglong)rowCount();
}

bool DataTableModel::allDataLoaded() const
{
    return !isLimited() || (_wantedRowsCount == meow::db::DATA_MAX_ROWS);
}

} // namespace db
} // namespace models
} // namespace meow
