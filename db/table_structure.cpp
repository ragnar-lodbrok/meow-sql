#include "table_structure.h"

namespace meow {
namespace db {

TableStructure::TableStructure()
    : _avgRowLen(0),
      _autoInc(0),
      _maxRows(0),
      _isCheckSum(false)
{

}

TableStructure::~TableStructure()
{
    qDeleteAll(_columns);
    qDeleteAll(_indicies);
    qDeleteAll(_foreignKeys);
}

bool TableStructure::hasIndexForColumn(
    const QString & columnName, TableIndexClass indexClass)
{
    for (auto & index : _indicies) {
        if (index->classType() == indexClass
            && index->columns().contains(columnName)) {
            return true;
        }
    }
    return false;
}

TableStructure * TableStructure::deepCopy() const
{
    TableStructure * structure = new TableStructure(*this);

    structure->_columns.clear();
    structure->_indicies.clear();
    structure->_foreignKeys.clear();

    for (auto & column : _columns) {
        TableColumn * columnCopy = new TableColumn(*column);
        structure->_columns.push_back(columnCopy);
    }

    for (auto & index : _indicies) {
        TableIndex * indexCopy = new TableIndex(*index);
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

    _columns.insert(newColumnIndex, newColumn);

    return newColumnIndex;
}

bool TableStructure::canRemoveColumn(int index) const
{
    return index >= 0 && index < _columns.size();
}

bool TableStructure::removeColumnAt(int index)
{
    TableColumn * columnToRemove = _columns.value(index, nullptr);

    if (columnToRemove) {
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

} // namespace db
} // namespace meow
