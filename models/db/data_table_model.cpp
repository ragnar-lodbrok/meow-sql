#include "data_table_model.h"
#include "db/query_data_fetcher.h"
#include "db/connection.h"
#include "db/common.h"
#include "db/query.h"
#include "db/query_criteria.h"
#include <QDebug>
#include "helpers/formatting.h"
#include "db/entity/table_entity.h"
#include <QColor>
#include "app/app.h"

namespace meow {
namespace models {
namespace db {

DataTableModel::DataTableModel(QObject *parent)
    :BaseDataTableModel(new meow::db::QueryData(), parent),
      _entityChangedProcessed(false),
      _dbEntity(nullptr),
      _wantedRowsCount(meow::db::DATA_MAX_ROWS)
{
    QObject::connect(queryData(), &meow::db::QueryData::editingPrepared,
            this, &DataTableModel::editingStarted);
}

DataTableModel::~DataTableModel()
{
    delete queryData(); // oh shit
}

Qt::ItemFlags DataTableModel::flags(const QModelIndex &index) const
{
    // TODO: allow editing in BaseDataTableModel?
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    Qt::ItemFlags flags = BaseDataTableModel::flags(index);
    flags |= Qt::ItemIsEditable; // TODO: read-only tables?

    return flags;
}

bool DataTableModel::setData(const QModelIndex &index,
                             const QVariant &value,
                             int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    if (queryData()->setData(index.row(), index.column(), value)) {
        emit dataChanged(index, index);
        return true;
    }

    return false;
}


void DataTableModel::setEntity(meow::db::Entity * tableOrViewEntity,
                               bool loadData)
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

    queryData()->clearData();
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

    auto textSettings = meow::app()->settings()->textSettings();
    bool limitDataLoadLen = textSettings->autoLimitLoadDataLength();

    if (limitDataLoadLen) {
        if (_dbEntity->type() == meow::db::Entity::Type::Table) {
            auto table = static_cast<meow::db::TableEntity *>(_dbEntity);
            queryCritera.select = queryDataFetcher->selectList(table);
        }
    }

    if (queryData()->query() == nullptr) {
        queryData()->setQueryPtr( // TODO: what a shitty code?
            _dbEntity->connection()->createQuery()
        );
        queryData()->query()->setEntity(_dbEntity);
    }
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

bool DataTableModel::isEditing()
{
    return (queryData()->query() && queryData()->query()->isEditing());
}

bool DataTableModel::isModified()
{
    return queryData()->isModified();
}

void DataTableModel::applyModifications()
{
    if (queryData() && queryData()->isModified()) {
        bool isInserted = queryData()->isInserted();
        int curRow = queryData()->currentRowNumber();
        int modifiedRow = queryData()->applyModifications();
        if (modifiedRow != -1) {
            emit dataChanged(
                index(modifiedRow, 0),
                index(modifiedRow, columnCount() - 1));
        } else {
            if (isInserted && curRow != -1) {
                beginRemoveRows(QModelIndex(), curRow, curRow);
                queryData()->deleteRow(curRow);
                endRemoveRows();
            }
        }
    }
}

void DataTableModel::discardModifications()
{
    if (queryData() && queryData()->isModified()) {
        bool isInserted = queryData()->isInserted();
        int curRow = queryData()->currentRowNumber();
        int modifiedRow = queryData()->discardModifications();
        if (modifiedRow != -1) {
            if (isInserted) {
                if (curRow != -1) {
                    beginRemoveRows(QModelIndex(), curRow, curRow);
                    queryData()->deleteRow(curRow);
                    endRemoveRows();
                }
            } else {
                emit dataChanged(
                    index(modifiedRow, 0),
                    index(modifiedRow, columnCount() - 1));
            }
        }
    }
}

void DataTableModel::setCurrentRowNumber(int row)
{
    queryData()->setCurrentRowNumber(row);
}

bool DataTableModel::deleteRowInDB(int row)
{
    if (queryData()->deleteRowInDB(row)) {
        beginRemoveRows(QModelIndex(), row, row);
        endRemoveRows();
        return true;
    }

    return false;
}

int DataTableModel::insertEmptyRow()
{
    int newRowIndex = queryData()->insertEmptyRow();
    if (newRowIndex != -1) {
        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
        endInsertRows();
    }
    return newRowIndex;
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
