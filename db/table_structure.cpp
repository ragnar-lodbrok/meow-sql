#include "table_structure.h"
#include <QDebug>

namespace meow {
namespace db {

TableStructure::TableStructure(TableEntity * table)
    : _table(table),
      _avgRowLen(0),
      _autoInc(0),
      _maxRows(0),
      _isCheckSum(false),
      _nextColumnUniqueId(0),
      _nextIndexUniqueId(0),
      _nextForeignKeyUniqueId(0)
{

}

TableStructure::~TableStructure()
{
    qDeleteAll(_columns);
    qDeleteAll(_indicies);
    qDeleteAll(_foreignKeys);
}

void TableStructure::setTable(TableEntity * table)
{
    _table = table;
    for (auto & index : _indicies) {
        index->setTable(table);
    }
}

bool TableStructure::hasIndexForColumn(
    const QString & columnName, TableIndexClass indexClass)
{
    for (auto & index : _indicies) {
        if (index->classType() == indexClass && index->hasColumn(columnName)) {
            return true;
        }
    }
    return false;
}

TableStructure * TableStructure::deepCopy(TableEntity * parentTable) const
{
    TableStructure * structure = new TableStructure(parentTable);

    structure->_table = parentTable;
    structure->_comment = this->_comment;
    structure->_rowFormatStr = this->_rowFormatStr;
    structure->_avgRowLen = this->_avgRowLen;
    structure->_autoInc = this->_autoInc;
    structure->_maxRows = this->_maxRows;
    structure->_isCheckSum = this->_isCheckSum;
    structure->_maxRows = this->_maxRows;
    structure->_nextColumnUniqueId = this->_nextColumnUniqueId;
    structure->_nextIndexUniqueId = this->_nextIndexUniqueId;
    structure->_nextForeignKeyUniqueId = this->_nextForeignKeyUniqueId;

    structure->_columns.clear();
    structure->_indicies.clear();
    structure->_foreignKeys.clear();

    for (auto & column : _columns) {
        TableColumn * columnCopy = new TableColumn(*column);
        structure->_columns.push_back(columnCopy);
    }

    for (auto & index : _indicies) {
        TableIndex * indexCopy = index->deepCopy(parentTable);
        structure->_indicies.push_back(indexCopy);
    }

    for (auto & fkey : _foreignKeys) {
        ForeignKey * fkeyCopy = new ForeignKey(*fkey);
        structure->_foreignKeys.push_back(fkeyCopy);
    }

    return structure;
}

int TableStructure::insertEmptyDefaultColumn(int afterIndex)
{
    TableColumn * newColumn = nullptr;

    TableColumn * parentColumn = _columns.value(afterIndex, nullptr);
    int newColumnIndex = afterIndex + 1;

    if (parentColumn == nullptr) {
        newColumnIndex = _columns.size();
        newColumn = new TableColumn();
        newColumn->setDataType(meow::db::DataTypeIndex::Int);
        newColumn->setAllowNull(true);
    } else {
        newColumn = new TableColumn(*parentColumn);
        if (parentColumn->defaultType() == ColumnDefaultType::AutoInc) {
            newColumn->setDefaultType(ColumnDefaultType::Text);
            newColumn->setDefaultText("0"); // why not empty?
        }
    }

    newColumn->setName(QString("column_%1").arg(newColumnIndex + 1));
    newColumn->setId(nextColumnUniqueId());

    _columns.insert(newColumnIndex, newColumn);

    return newColumnIndex;
}

TableIndex * TableStructure::insertEmptyDefaultIndex()
{
    TableIndex * newIndexObj = new TableIndex(_table);
    newIndexObj->setId(nextIndexUniqueId());
    newIndexObj->setName(QString("index_%1").arg(_indicies.size() + 1));

    if (_indicies.size() == 0) {
        newIndexObj->setClassType(TableIndexClass::PrimaryKey);
    } else {
        newIndexObj->setClassType(TableIndexClass::Key);
    }

    _indicies.append(newIndexObj);
    return newIndexObj;
}

int TableStructure::insertEmptyDefaultColumnToIndex(int index)
{
    if (!isValidIndex(index)) {
        return -1;
    }

    TableIndex * tableIndex = _indicies[index];

    for (const auto & column : _columns) {
        if (!tableIndex->hasColumn(column->name())) {
            return tableIndex->addColumn(column->name());
        }
    }

    return -1;
}

int TableStructure::insertEmptyDefaultForeignKey()
{
    ForeignKey * newKeyObj = new ForeignKey(/*_table*/);
    _foreignKeys.append(newKeyObj);
    int newKeyIndex = _foreignKeys.size() - 1;
    newKeyObj->setName(QString("fk_%1").arg( newKeyIndex + 1 ));
    newKeyObj->setIsCustomName(false);
    newKeyObj->setId(nextForeignKeyUniqueId());

    return newKeyIndex;
}

void TableStructure::appendColumn(TableColumn * column)
{
    // We use unique ids for columns/indices/etc for same objects in different
    // structures e.g. after copying them to edit. This way we can say that two
    // objects represent the same column in db even a user changed one of it's
    // name
    column->setId(nextColumnUniqueId());
    _columns.append(column);
}

void TableStructure::appendIndex(TableIndex * index)
{
    index->setId(nextIndexUniqueId());
    _indicies.append(index);
}

bool TableStructure::canRemoveColumn(int index) const
{
    return index >= 0 && index < _columns.size();
}

bool TableStructure::removeColumnAt(int index)
{
    TableColumn * columnToRemove = _columns.value(index, nullptr);

    if (columnToRemove) {
        emit beforeColumnRemoved(columnToRemove->name());
        _columns.removeAt(index);
        delete columnToRemove;
        return true;
    }
    return false;
}

bool TableStructure::canMoveColumnUp(int index) const
{
    return index >= 1 && index < _columns.size();
}

bool TableStructure::moveColumnUp(int index)
{
    if (canMoveColumnUp(index)) {
        _columns.move(index, index - 1);
        return true;
    }
    return false;
}

bool TableStructure::canMoveColumnDown(int index) const
{
    return index >= 0 && index < (_columns.size() - 1);
}

bool TableStructure::moveColumnDown(int index)
{
    if (canMoveColumnDown(index)) {
        _columns.move(index, index + 1);
        return true;
    }
    return false;
}

bool TableStructure::canRemoveAllIndices() const
{
    return _indicies.size() > 0;
}

void TableStructure::removeAllIndicies()
{
    qDeleteAll(_indicies);
    _indicies.clear();
}

bool TableStructure::isValidIndex(int index) const
{
    return index >= 0 && index < _indicies.size();
}

bool TableStructure::canRemoveIndex(int index) const
{
    return isValidIndex(index);
}

bool TableStructure::removeIndexAt(int index)
{
    if (!canRemoveIndex(index)) return false;

    TableIndex * indexToRemove = _indicies.value(index, nullptr);

    if (indexToRemove) {
        _indicies.removeAt(index);
        delete indexToRemove;
        return true;
    }
    return false;
}

bool TableStructure::canRemoveIndexColumn(int index, int column) const
{
    if (isValidIndex(index)) {
        return _indicies[index]->isValidColumnIndex(column);
    }
    return false;
}

bool TableStructure::removeIndexColumn(int index, int column)
{
    if (!canRemoveIndexColumn(index, column)) return false;
    return _indicies[index]->removeColumnIndex(column);
}

bool TableStructure::canMoveIndexColumnUp(int index, int column) const
{
    if (isValidIndex(index)) {
        return _indicies[index]->canMoveColumnUp(column);
    }
    return false;
}

bool TableStructure::canMoveIndexColumnDown(int index, int column) const
{
    if (isValidIndex(index)) {
        return _indicies[index]->canMoveColumnDown(column);
    }
    return false;
}

bool TableStructure::isValidKey(int index) const
{
    return index >= 0 && index < _foreignKeys.size();
}

bool TableStructure::canRemoveKey(int index) const
{
    return isValidKey(index);
}

bool TableStructure::removeKeyAt(int index)
{
    ForeignKey * keyToRemove = _foreignKeys.value(index, nullptr);

    if (keyToRemove) {
        _foreignKeys.removeAt(index);
        delete keyToRemove;
        return true;
    }
    return false;
}

bool TableStructure::canRemoveAllKeys() const
{
    return _foreignKeys.size() > 0;
}

void TableStructure::removeAllKeys()
{
    qDeleteAll(_foreignKeys);
    _foreignKeys.clear();
}

TableColumn * TableStructure::columnById(unsigned id) const
{
    for (const auto & column : _columns) {
        if (column->id() == id) {
            return column;
        }
    }
    return nullptr;
}

TableColumn * TableStructure::columnByName(const QString & name) const
{
    for (const auto & column : _columns) {
        if (column->name() == name) {
            return column;
        }
    }
    return nullptr;
}

TableColumn * TableStructure::prevColumn(TableColumn * column) const
{
    return _columns.value(_columns.indexOf(column) - 1, nullptr);
}

TableIndex * TableStructure::indexById(unsigned id) const
{
    for (const auto & index : _indicies) {
        if (index->id() == id) {
            return index;
        }
    }
    return nullptr;
}

QStringList tableColumnNames(TableStructure * structure) {
    QStringList names;
    const QList<TableColumn *> & columns = structure->columns();
    for (const auto & column : columns) {
        names.append(column->name());
    }
    return names;
}

} // namespace db
} // namespace meow
