#include "query.h"
#include "exception.h"
#include "editable_grid_data.h"
#include "entity/table_entity.h"
#include <QDebug>

namespace meow {
namespace db {

Query::Query(Connection * connection)
    :_recordCount(0),
     _curRecNo(-1),
     _eof(false),
     _editableData(nullptr),
     _connection(connection),
     _entity(nullptr)
{

}

Query::~Query()
{
    delete _editableData;
}

void Query::setSQL(const QString & SQL)
{
    _SQL = SQL;
}

void Query::seekFirst()
{
    _curRecNo = -1;
    seekRecNo(0);
}

void Query::seekNext()
{
    seekRecNo(_curRecNo + 1);
}

db::ulonglong Query::recordCount() const
{
    return isEditing() ? _editableData->rowsCount() : _recordCount;
}

QString Query::curRowColumn(const QString & colName, bool ignoreErrors /* = false */)
{
    std::size_t index;

    if (ignoreErrors) {
        try {
            index = indexOfColumn(colName);
        } catch(meow::db::Exception & ex) {
            qDebug() << "[Query] " << "Failed to find column by name: " << ex.message();
            return QString();
        }
    } else {
        index = indexOfColumn(colName);
    }

    return curRowColumn(index, ignoreErrors);
}

QStringList Query::curRow()
{
    QStringList rowData;
    std::size_t columnCount = this->columnCount();
    for (std::size_t i = 0; i < columnCount; ++i) {
        if (isNull(i)) {
            rowData << QString();
        } else {
            rowData << curRowColumn(i);
        }
    }
    return rowData;
}

std::size_t Query::indexOfColumn(const QString & colName) const
{
    if (_columnIndexes.contains(colName)) {
        return _columnIndexes.value(colName);
    } else if (_columnIndexes.contains(colName.toLower())) {
        return _columnIndexes.value(colName.toLower());
    } else {
        throw db::Exception(
            QString("Column \"%1\" not available.").arg(colName)
        );
    }
}

bool Query::prepareEditing()
{
    if (_editableData) {
        return true;
    }
    _editableData = new EditableGridData();
    return false;
}

QStringList Query::keyColumns() const
{
    Q_ASSERT(entity());

    QStringList cols;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        QList<TableIndex *> & indicies = table->structure()->indicies();
        for (const auto & index : indicies) {
            if (index->isPrimaryKey()) {
                cols.append(index->columnNames());
            }
        }

    } else {
        // TODO
        Q_ASSERT(false);
    }

    return cols;
}

} // namespace db
} // namespace meow

