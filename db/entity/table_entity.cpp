#include "table_entity.h"
#include "database_entity.h"
#include "db/connection.h"
#include <QIcon>
#include <QDebug>

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

DataBaseEntity * TableEntity::database() const
{
    return static_cast<DataBaseEntity *>(parent());
}

TableEntity * TableEntity::deepCopy() const
{
    TableEntity * copy = new TableEntity(_tableName, database());

    copy->copyDataFrom(this);

    return copy;
}

void TableEntity::copyDataFrom(const Entity * data)
{
    Q_ASSERT(data->type() == Entity::Type::Table);

    const TableEntity * table = static_cast<const TableEntity *>(data);

    EntityInDatabase::copyDataFrom(table);

    this->_collation = table->_collation;
    this->_tableName = table->_tableName;
    this->_engineStr = table->_engineStr;

    this->_rowsCount = table->_rowsCount;
    this->_dataSize  = table->_dataSize;
    this->_version   = table->_version;

    delete this->_structure;
    this->_structure = nullptr;

    if (table->_structure) {
        this->_structure = table->_structure->deepCopy(this);
        //this->_structure->setTable(this);
    }
}

} // namespace db
} // namespace meow
