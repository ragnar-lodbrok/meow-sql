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
namespace ui {
namespace models {

DataTableModel::DataTableModel(QObject *parent)
    : BaseDataTableModel(
          meow::db::QueryDataPtr(new meow::db::QueryData()),
          parent),
      _sortFilterModel(nullptr),
      _filterPatternIsRegexp(false),
      _entityChangedProcessed(false),
      _dbEntity(nullptr),
      _wantedRowsCount(meow::db::DATA_MAX_ROWS)
{
    QObject::connect(queryData(), &meow::db::QueryData::editingPrepared,
            this, &DataTableModel::editingStarted);
}

DataTableModel::~DataTableModel()
{

}

Qt::ItemFlags DataTableModel::flags(const QModelIndex &index) const
{
    // TODO: allow editing in BaseDataTableModel?
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    Qt::ItemFlags flags = BaseDataTableModel::flags(index);

    bool isEditable = false;
    if (_dbEntity) {
        isEditable =
            _dbEntity->connection()->features()->supportsEditingTablesData();
    }

    if (isEditable) {
        flags |= Qt::ItemIsEditable; // TODO: read-only tables?
    }


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

delegates::EditorType DataTableModel::editorType(
        const QModelIndex &index) const
{
    if (meow::app()->settings()->dataEditors()
            ->enableDropDownForForeignKeyEditors()
            && columnHasForeignKey(index.column())) {
        return delegates::EditorType::comboboxEdit;
    }

    if (typeCategoryForColumn(index.column())
            == meow::db::DataTypeCategoryIndex::Text) {
        return delegates::EditorType::lineEdit;
    } else if (typeCategoryForColumn(index.column())
               == meow::db::DataTypeCategoryIndex::Temporal) {

        if (meow::app()->settings()->dataEditors()
                ->enableInplaceDatetimeEditor()) {

            bool isDateTime= connection()->dataTypes()->isDateTimeType(
                        dataTypeForColumn(index.column()));

            if (isDateTime) {
                return delegates::EditorType::dateTimeEdit;
            }

            // TODO: date, time, year editors ...

        }
    }

    return delegates::EditorType::defaultEditor;
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
    int rowCount = this->rowCount();
    int columnCount = this->columnCount();

    if (rowCount) {
        beginRemoveRows(QModelIndex(), 0, rowCount-1);
        setRowCount(0);
        endRemoveRows();
    }

    if (columnCount) {
        beginRemoveColumns(QModelIndex(), 0, columnCount-1);
        setColumnCount(0);
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

    int newColumnCount = queryData()->columnCount();

    if (newColumnCount > prevColCount) {
        beginInsertColumns(QModelIndex(), prevColCount, newColumnCount-1);
        setColumnCount(newColumnCount); // tell the model we have change in data
        endInsertColumns();
    }

    int newRowCount = queryData()->rowCount();

    if (newRowCount > prevRowCount) {
        // Qt models require actual change in data to happen between
        // beginInsertRows and endInsertRows. Since we got data from query
        // emulate this by settings row count by hand. Otherwise proxy models
        // go crazy.
        beginInsertRows(QModelIndex(), prevRowCount, newRowCount-1);
        setRowCount(newRowCount);
        endInsertRows();
    }
}

bool DataTableModel::isEditable() const
{
    if (_dbEntity == nullptr) {
        return false;
    }
    if (_dbEntity->type() == meow::db::Entity::Type::Table) {
        return _dbEntity->connection()->features()->supportsEditingTablesData();
    }
    if (_dbEntity->type() == meow::db::Entity::Type::View) {
        return _dbEntity->connection()->features()->supportsEditingViewsData();
    }
    return false;
}

bool DataTableModel::isEditing()
{
    return (queryData()->query() && queryData()->query()->isEditing());
}

bool DataTableModel::isModified()
{
    return queryData()->isModified();
}

void DataTableModel::applyModifications(int rowToApply)
{
    if (queryData() && queryData()->isModified()) {
        bool isInserted = queryData()->isInserted();
        int curRow = queryData()->currentRowNumber();
        rowToApply = (rowToApply != -1) ? rowToApply : curRow;
        int modifiedRow = queryData()->applyModifications();
        if (modifiedRow != -1) {
            emit dataChanged(
                index(modifiedRow, 0),
                index(modifiedRow, columnCount() - 1));
        } else {
            if (isInserted && rowToApply != -1) {
                int rowCount = this->rowCount();
                beginRemoveRows(QModelIndex(), rowToApply, rowToApply);
                queryData()->deleteRow(rowToApply);
                setRowCount(rowCount - 1);
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
                    int rowCount = this->rowCount();
                    beginRemoveRows(QModelIndex(), curRow, curRow);
                    queryData()->deleteRow(curRow);
                    setRowCount(rowCount - 1);
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
    int oldRowCount = rowCount();
    if (queryData()->deleteRowInDB(row)) {
        beginRemoveRows(QModelIndex(), row, row);
        setRowCount(oldRowCount-1);
        endRemoveRows();
        return true;
    }

    return false;
}

int DataTableModel::insertEmptyRow()
{
    return insertNewRow();
}

int DataTableModel::duplicateCurrentRowWithoutKeys()
{
    return insertNewRow(true, false);
}

int DataTableModel::duplicateCurrentRowWithKeys()
{
    return insertNewRow(true, true);
}

int DataTableModel::insertNewRow(bool duplicateCurrent, bool withKeys)
{
    int oldRowCount = rowCount();
    int newRowIndex = -1;
    if (duplicateCurrent) {
        if (withKeys) {
            newRowIndex = queryData()->duplicateCurrentRowWithKeys();
        } else {
            newRowIndex = queryData()->duplicateCurrentRowWithoutKeys();
        }
    } else {
        Q_ASSERT(withKeys != true); // Not implemented
        newRowIndex = queryData()->insertEmptyRow();
    }
    if (newRowIndex != -1) {
        beginInsertRows(QModelIndex(), newRowIndex, newRowIndex);
        setRowCount(oldRowCount+1);
        endInsertRows();
    }
    return newRowIndex;
}

QAbstractItemModel * DataTableModel::createSortFilterModel()
{
    if (_sortFilterModel == nullptr) {
        _sortFilterModel = new QueryDataSortFilterProxyModel(queryData(), this);
        _sortFilterModel->setSourceModel(this);
        _sortFilterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        _sortFilterModel->setFilterKeyColumn(-1); // all columns

        setFilterPattern(_filterPattern, _filterPatternIsRegexp);
    }
    return _sortFilterModel;
}

void DataTableModel::setFilterPattern(const QString & pattern, bool regexp)
{
    if (_filterPattern == pattern && _filterPatternIsRegexp == regexp) return;

    _filterPattern = pattern;
    _filterPatternIsRegexp = regexp;
    if (_sortFilterModel) {
        if (_filterPatternIsRegexp) {
            _sortFilterModel->setFilterRegExp(QRegExp(pattern,
                                                      Qt::CaseInsensitive,
                                                      QRegExp::RegExp));
        } else {
            _sortFilterModel->setFilterWildcard(pattern);
        }
    }
}

QString DataTableModel::filterPattern() const
{
    return _filterPattern;
}

int DataTableModel::filterMatchedRowCount() const
{
    if (_sortFilterModel) {
        return _sortFilterModel->rowCount();
    } else {
        return rowCount(); // all matched if no filter
    }
}

void DataTableModel::refresh()
{
    removeData();
    loadData(true);
}

void DataTableModel::invalidateData()
{
    setEntity(_dbEntity, false);
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

} // namespace models
} // namespace ui
} // namespace meow
