#include "data_table_model.h"
#include "db/query_data_fetcher.h"
#include "db/connection.h"
#include "db/common.h"
#include "db/query.h"
#include "db/query_criteria.h"
#include <QDebug>
#include "helpers/formatting.h"
#include "db/entity/table_entity.h"
#include "db/entity/view_entity.h"
#include <QColor>
#include "app/app.h"

namespace meow {
namespace ui {
namespace models {

DataTableModel::DataTableModel(QObject *parent)
    : BaseDataTableModel(
          meow::db::QueryDataPtr(new meow::db::QueryData()),
          parent),
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

    meow::db::DataTypeCategoryIndex typeCategory
            = typeCategoryForColumn(index.column());

    if (typeCategory == meow::db::DataTypeCategoryIndex::Text) {
        return delegates::EditorType::lineEdit;
    } else if (typeCategory == meow::db::DataTypeCategoryIndex::Temporal) {

        if (meow::app()->settings()->dataEditors()
                ->enableInplaceDatetimeEditor()) {

            meow::db::DataTypePtr type = dataTypeForColumn(index.column());

            if (connection()->dataTypes()->isDateTimeType(type)) {
                return delegates::EditorType::dateTimeEdit;
            } else if (connection()->dataTypes()->isDateType(type)) {
                return delegates::EditorType::dateEdit;
            } else if (connection()->dataTypes()->isTimeType(type)) {
                return delegates::EditorType::timeEdit;
            } else if (connection()->dataTypes()->isYearType(type)) {
                return delegates::EditorType::yearEdit;
            }
        }
    } else if (typeCategory == meow::db::DataTypeCategoryIndex::Other) {

        meow::db::DataTypePtr type = dataTypeForColumn(index.column());

        if (connection()->dataTypes()->isEnumType(type)
                && meow::app()->settings()->dataEditors()
                    ->enableInplaceEnumEditor()) {
            return delegates::EditorType::comboboxEdit;

        } else if (connection()->dataTypes()->isSetType(type)
                   && meow::app()->settings()->dataEditors()
                       ->enableInplaceSetEditor()) {
            return delegates::EditorType::checkboxListEdit;

        }
    }

    return delegates::EditorType::defaultEditor;
}

QVariant DataTableModel::headerData(int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    switch (role) {

    case Qt::DecorationRole:
        if (isColumnSorted(section)) {
            return (columnSortOrder(section) == Qt::AscendingOrder)
                    ? QIcon(":/icons/sort_ascending.png")
                    : QIcon(":/icons/sort_descending.png");
        }
        break;

    default:
        break;
    }


    return BaseDataTableModel::headerData(section, orientation, role);
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
    queryCritera.where = _whereFilter;
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

    if (!_columnsSort.empty()) {

        QStringList columnNames;

        if (_dbEntity->type() == meow::db::Entity::Type::Table) {
            auto table = static_cast<meow::db::TableEntity *>(_dbEntity);
            columnNames = table->structure()->columnNames();
        } else if (_dbEntity->type() == meow::db::Entity::Type::View) {
            auto view = static_cast<meow::db::ViewEntity *>(_dbEntity);
            columnNames = view->structure()->columnNames();
        }

        for (const SortColumn & sort : _columnsSort) {

            int columnIndex = sort.columnIndex;
            bool isAscending = (sort.sortOrder == Qt::AscendingOrder);

            if (columnIndex < columnNames.size()) {

                db::QueryCriteria::SortColumn sort;
                sort.columnName = columnNames[columnIndex];
                sort.isAsc = isAscending;

                queryCritera.sortColumns.push_back(sort);
            }
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

QList<db::TableColumn *> DataTableModel::selectedTableColumns()
{
    if (_dbEntity == nullptr) {
        return {};
    }
    if (_dbEntity->type() == meow::db::Entity::Type::Table) {
        db::TableEntity * table = static_cast<db::TableEntity *>(_dbEntity);
        return table->structure()->columns();
    }
    if (_dbEntity->type() == meow::db::Entity::Type::View) {
        db::ViewEntity * view = static_cast<db::ViewEntity *>(_dbEntity);
        return view->structure()->columns();
    }
    return {};
}

void DataTableModel::changeColumnSort(int columnIndex)
{
    bool found = false;
    auto it = _columnsSort.begin();
    while (it != _columnsSort.end()) {
        if ((*it).columnIndex == columnIndex) {
            if ((*it).sortOrder == Qt::AscendingOrder) {
                (*it).sortOrder = Qt::DescendingOrder;
            } else {
                _columnsSort.erase(it);
            }
            found = true;
            break;
        }
        ++it;
    }

    if (!found) {
        SortColumn sortColumn;
        sortColumn.columnIndex = columnIndex;
        sortColumn.sortOrder = Qt::AscendingOrder;
        _columnsSort.push_back(sortColumn); // order is important
    }

    // TODO: save sorted columns?
}

bool DataTableModel::isColumnSorted(int columnIndex) const
{
    for (const SortColumn & sort : _columnsSort) {
        if (sort.columnIndex == columnIndex) {
            return true;
        }
    }
    return false;
}

Qt::SortOrder DataTableModel::columnSortOrder(int columnIndex) const
{
    for (const SortColumn & sort : _columnsSort) {
        if (sort.columnIndex == columnIndex) {
            return sort.sortOrder;
        }
    }
    return Qt::AscendingOrder;
}

void DataTableModel::resetAllColumnsSort()
{
    _columnsSort.clear();
    // TODO: save sorted columns?
}

void DataTableModel::applyColumnSort()
{
    incRowsCountForOneStep(true); // reset limit/offset
    refresh();
}

void DataTableModel::refresh()
{
    removeData();
    loadData(true);
    emit dataRefreshed();
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

        meow::db::ulonglong rowsTotal = 0;
        if (_entityChangedProcessed && !isLimited() && !isFiltered()) {
            rowsTotal = rowCount();
        } else {
            rowsTotal = table->rowsCount(true);// TODO: rm extra query
        }

        result += ": ";

        if (table->engineStr() == "InnoDB") {
            result += "~"; // H: approximately
        }

        result += meow::helpers::formatNumber(rowsTotal) + " ";

        result += QObject::tr("rows total");

        if (isLimited()) {
            result += ", " + QObject::tr("limited to");
            result += " " + meow::helpers::formatNumber(rowCount());
        } else if (isFiltered()) {
            if (rowsTotal == (meow::db::ulonglong)rowCount()) {
                result += ", " + QObject::tr("all rows match to filter");
            } else {
                result += ", " + meow::helpers::formatNumber(rowCount())
                    + " " + QObject::tr("rows match to filter");
            }
        }

        // Listening: Alyans - Na Zare
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

bool DataTableModel::isFiltered() const
{
    return !_whereFilter.isEmpty();
}

bool DataTableModel::allDataLoaded() const
{
    return !isLimited() || (_wantedRowsCount == meow::db::DATA_MAX_ROWS);
}

void DataTableModel::applyWhereFilter(const QString & whereFilter)
{
    incRowsCountForOneStep(true); // reset limit/offset

    bool changed = _whereFilter != whereFilter;
    _whereFilter = whereFilter;
    if (changed) {
        refresh();
    }
}

void DataTableModel::resetWhereFilter()
{
    _whereFilter.clear();
}

} // namespace models
} // namespace ui
} // namespace meow
