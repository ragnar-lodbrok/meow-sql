#include "native_query_result.h"
#include "exception.h"
#include "editable_grid_data.h"
#include "entity/table_entity.h"
#include "helpers/logger.h"

namespace meow {
namespace db {

NativeQueryResult::NativeQueryResult(Connection * connection)
    :_recordCount(0),
     _curRecNo(-1),
     _eof(false),
     _editableData(nullptr),
     _connection(connection),
     _entity(nullptr)
{

}

NativeQueryResult::~NativeQueryResult()
{
    delete _editableData;
}

void NativeQueryResult::seekFirst()
{
    _curRecNo = -1;
    seekRecNo(0);
}

void NativeQueryResult::seekNext()
{
    seekRecNo(_curRecNo + 1);
}

db::ulonglong NativeQueryResult::recordCount() const
{
    return isEditing() ? _editableData->rowsCount() : _recordCount;
}

QString NativeQueryResult::curRowColumn(const QString & colName,
                                        bool ignoreErrors /* = false */)
{
    std::size_t index;

    if (ignoreErrors) {
        try {
            index = indexOfColumn(colName);
        } catch(meow::db::Exception & ex) {
            meowLogC(Log::Category::Error)
                << "Failed to find column by name: " << ex.message();
            return QString();
        }
    } else {
        index = indexOfColumn(colName);
    }

    return curRowColumn(index, ignoreErrors);
}

QStringList NativeQueryResult::curRow()
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

QMap<QString, QString> NativeQueryResult::curRowAsObject()
{
    QMap<QString, QString> object;

    std::size_t columnCount = this->columnCount();
    for (std::size_t i = 0; i < columnCount; ++i) {
        QString value = isNull(i) ? QString() : curRowColumn(i);
        object[columnName(i)] = value;
    }

    return object;
}

std::size_t NativeQueryResult::indexOfColumn(const QString & colName) const
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

bool NativeQueryResult::columnExists(const QString & colName) const
{
    if (_columnIndexes.contains(colName)) {
        return true;
    } else if (_columnIndexes.contains(colName.toLower())) {
        return true;
    }
    return false;
}

bool NativeQueryResult::prepareEditing()
{
    if (_editableData) {
        return true;
    }
    _editableData = new EditableGridData();

    prepareResultForEditing(this);

    for (const QueryResultPt & appendedResult : _appendedResults) {
        prepareResultForEditing(appendedResult.get());
    }

    return false;
}

QStringList NativeQueryResult::keyColumns() const
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

QList<std::size_t> NativeQueryResult::primaryColumnIndices() const
{
    // Listening: Infected Rain = Fighter
    QList<std::size_t> pkIndices;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        // Primary Keys -----------------------------------------------------
        QList<TableIndex *> & indicies = table->structure()->indicies();
        for (const auto & index : indicies) {

            if (!index->isPrimaryKey()) continue;

            for (const QString & pkColumnName : index->columnNames()) {
                if (_columnIndexes.contains(pkColumnName)) {
                    size_t pkColumnIndex = _columnIndexes.value(pkColumnName);
                    if (!pkIndices.contains(pkColumnIndex)) {
                        pkIndices.push_back(pkColumnIndex);
                    }
                }
            }
        }
    }

    return pkIndices;
}

QList<ForeignKey *> NativeQueryResult::foreignKeysForColumn(std::size_t index)
{
    if (!entity()) return {};

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        return table->structure()->foreignKeysForColumn(columnName(index));
    }

    return {};
}

QStringList NativeQueryResult::columnValuesList(std::size_t index) const
{
    if (!entity()) return {};

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        QString lenSet = table->structure()->columns()[index]->lengthSet();

        QStringList splittedList = lenSet.split(',');
        QStringList result;
        for (QString str : splittedList) {
            int left = str.startsWith('\'') ? 1 : 0;
            int len = str.length() - (str.endsWith('\'') ? 1 : 0) - left;
            result << str.mid(left, len);
        }
        return result;
    }

    return {};
}

void NativeQueryResult::throwOnInvalidColumnIndex(std::size_t index)
{
    if (index >= columnCount()) {
        throw db::Exception(
            QString("Column #%1 not available. Query returned %2 columns.")
                    .arg(index).arg(columnCount())
        );
    }
}

void NativeQueryResult::appendResultData(const QueryResultPt &result)
{
    if (isEditing()) {
        prepareResultForEditing(result.get());
    } else {
        _appendedResults.push_back(result);
        _recordCount += result->recordCount();
        if (_entity) {
            result->setEntity(_entity);
        }
    }
}

bool NativeQueryResult::columnIsPrimaryKeyPart(std::size_t index) const
{
    if (!entity()) return false;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        return table->structure()->isColumnPrimaryKey(index);
    }

    return false;
}

bool NativeQueryResult::columnIsUniqueKeyPart(std::size_t index) const
{
    if (!entity()) return false;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        return table->structure()->isColumnUniqueKey(index);
    }

    return false;
}

bool NativeQueryResult::columnIsIndexKeyPart(std::size_t index) const
{
    if (!entity()) return false;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        return table->structure()->isColumnIndexKey(index);
    }

    return false;
}

bool NativeQueryResult::columnIsAutoIncrement(std::size_t index) const
{
    if (!entity()) return false;

    if (entity()->type() == Entity::Type::Table) {
        TableEntity * table = static_cast<TableEntity *>(entity());

        return table->structure()->columnIsAutoIncrement(index);
    }

    // TODO: not for table only

    return false;
}

} // namespace db
} // namespace meow
