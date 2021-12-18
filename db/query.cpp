#include "query.h"
#include "exception.h"
#include "editable_grid_data.h"
#include "entity/table_entity.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

Query::Query(Connection * connection)
    :_recordCount(0),
     _curRecNo(-1),
     _rowsFound(0),
     _rowsAffected(0),
     _warningsCount(0),
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
            meowLogCC(Log::Category::Error, _connection)
                << "Failed to find column by name: " << ex.message();
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

QMap<QString, QString> Query::curRowAsObject()
{
    QMap<QString, QString> object;

    std::size_t columnCount = this->columnCount();
    for (std::size_t i = 0; i < columnCount; ++i) {
        QString value = isNull(i) ? QString() : curRowColumn(i);
        object[columnName(i)] = value;
    }

    return object;
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
    // TODO: cache?

    Q_ASSERT(entity());

    QStringList cols;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        // Primary Keys -----------------------------------------------------
        QList<TableIndex *> & indicies = table->structure()->indicies();
        for (const auto & index : indicies) {
            if (index->isPrimaryKey()) {
                cols.append(index->columnNames());
                // TODO: why just don't return here?
            }
        }

        if (!cols.isEmpty()) {
            return cols;
        }

        // if no PKs check UNIQUE keys --------------------------------------
        for (const auto & index : indicies) {
            if (!index->isUniqueKey()) {
                continue;
            }
            // Check if no column in UNIQUE key allows NULL
            // which makes it dangerous to use in UPDATES + DELETES
            if (index->hasColumnsWithAllowNull() == false) {
                if (index->columnsCount() > 0) {
                    return index->columnNames();
                }
            }
        }

        // no PK and no good UNIQUE keys - just use all columns -------------
        return table->structure()->columnNames();

    } else {
        // TODO
        Q_ASSERT(false);
    }

    return cols;
}

} // namespace db
} // namespace meow

