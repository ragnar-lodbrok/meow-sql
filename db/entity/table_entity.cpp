#include "table_entity.h"
#include "database_entity.h"
#include <QIcon>

namespace meow {
namespace db {

TableEntity::TableEntity(const QString & tableName, DataBaseEntity * parent)
    :EntityInDatabase(parent),
     _tableName(tableName),
     _rowsCount(0),
     _dataSize(0),
     _version(0),
     _structure(nullptr)
{

}

TableEntity::~TableEntity()
{
    delete _structure;
}

QString TableEntity::name() const // override
{
    return _tableName;
}

QVariant TableEntity::icon() const // override
{
    // TODO: move to Settings::icons
    static const QIcon icon = QIcon(":/icons/table.png");

    return icon;
}

db::ulonglong TableEntity::rowsCount(bool refresh /*= false*/)
{
    if (refresh) {
        _rowsCount = connection()->getRowCount(this);
    }
    return _rowsCount;
}

db::TableStructure * TableEntity::structure() const
{
    if (!_structure) {
        _structure = new TableStructure(const_cast<TableEntity *>(this));
    }
    return _structure;
}

bool TableEntity::hasStructure() const
{
    return _structure != nullptr;
}

TableEntity * TableEntity::deepCopy() const
{
    TableEntity * copy = new TableEntity(*this);
    if (this->_structure) {
        copy->_structure = this->_structure->deepCopy(copy);
        //copy->_structure->setTable(copy);
    }

    return copy;
}

void TableEntity::copyData(TableEntity * data)
{
    delete this->_structure;
    *this = *data;
    if (data->_structure) {
        this->_structure = data->_structure->deepCopy(this);
        //this->_structure->setTable(this);
    }
}

} // namespace db
} // namespace meow
