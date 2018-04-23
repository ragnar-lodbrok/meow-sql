#include "mysql_table_editor.h"
#include "mysql_connection.h"
#include "db/entity/table_entity_comparator.h"
#include "db/entity/table_entity.h"
#include <QDebug>

namespace meow {
namespace db {

MySQLTableEditor::MySQLTableEditor(MySQLConnection * connection)
    :TableEditor(connection)
{

}

bool MySQLTableEditor::edit(TableEntity * table, TableEntity * newData)
{
    bool changed = false;

    // TODO: begin transaction ?

    TableEntityComparator diff;
    diff.setCurrTable(newData);
    diff.setPrevTable(table);

    // Columns -----------------------------------------------------------------

    auto modifiedColumns = diff.modifiedColumns();
    for (auto & modifiedColumnPair : modifiedColumns) {

        // we can DROP DEFAULT in separate query only

        auto newColDef = modifiedColumnPair.newCol->defaultType();
        auto oldColDef = modifiedColumnPair.oldCol->defaultType();

        if (oldColDef != newColDef && newColDef == ColumnDefaultType::None) {
            QString SQL = QString("ALTER TABLE %1 ALTER %2 DROP DEFAULT;")
                    .arg(db::quotedName(table))
                    .arg(_connection->quoteIdentifier(
                             modifiedColumnPair.oldCol->name()));

            _connection->query(SQL);
            changed = true;
        }
    }

    QStringList specs = this->specs(table, newData);

    TableColumn * prevColumn = nullptr;

    auto columsStatuses = diff.currColumnsWithStatus();
    for (auto & columnStatus : columsStatuses) {

        if (columnStatus.modified == false && columnStatus.added == false) {
            prevColumn = columnStatus.columns.newCol;
            continue;
        }

        TableColumn * column = columnStatus.columns.newCol;
        QString columnSt = sqlCode(column);

        if (_connection->serverVersionInt() >= 40001) {
            if (prevColumn) {
                columnSt += " AFTER "
                         + _connection->quoteIdentifier(prevColumn->name());
            } else {
                columnSt += " FIRST";
            }
        }

        if (columnStatus.modified) {
            TableColumn * oldColumn = columnStatus.columns.oldCol;
            QString alterSt = alterColumnSQL(
                _connection->quoteIdentifier(oldColumn->name()),
                columnSt);
            specs << alterSt;
        } else if (columnStatus.added) {
            QString addSt = QString("ADD COLUMN %1").arg(columnSt);
            specs << addSt;
        }

        prevColumn = columnStatus.columns.newCol;
    }

    auto droppedColumns = diff.removedColumns();
    for (const TableColumn * droppedColumn : droppedColumns) {
        QString dropSt = QString("DROP COLUMN %1")
                .arg(_connection->quoteIdentifier(droppedColumn->name()));
        specs << dropSt;
    }

    // Indices -----------------------------------------------------------------

    auto droppedIndices = diff.removedIndices();
    for (const TableIndex * droppedIndex : droppedIndices) {
        specs << dropSQL(droppedIndex);
    }

    auto indexStatuses = diff.currIndicesWithStatus();
    for (const auto & indexStatus : indexStatuses) {

        if (indexStatus.modified == false && indexStatus.added == false) {
            continue;
        }

        if (indexStatus.modified) {
            specs << dropSQL(indexStatus.oldIndex);
        }
        if (indexStatus.added || indexStatus.modified) {
            QString SQL = sqlCode(indexStatus.newIndex);
            if (!SQL.isEmpty()) {
                specs << "ADD " + SQL;
            }
        }
    }

    // -------------------------------------------------------------------------

    if (!specs.isEmpty()) {
        QString joinedSpecs = specs.join(",\n");

        QString alterSt = QString("ALTER TABLE %1\n%2;")
                .arg(db::quotedName(table))
                .arg(joinedSpecs);

        _connection->query(alterSt);
        changed = true;
    }

    if (diff.nameDiffers()) {
        rename(table, newData->name());
        changed = true;
    }

    // TODO: end transaction ?

    return changed;
}

bool MySQLTableEditor::insert(TableEntity * table)
{
    QString SQL = QString("CREATE TABLE %1").arg(db::quotedName(table));

    QStringList specs;

    const QList<TableColumn *> & columns = table->structure()->columns();

    for (auto & column : columns) {
        specs << sqlCode(column);
    }

    const QList<TableIndex *> & indices = table->structure()->indicies();
    for (auto & index : indices) {
        QString SQL = sqlCode(index);
        if (!SQL.isEmpty()) {
            specs << SQL;
        }
    }

    SQL += QString(" (\n%1\n)\n").arg(specs.join(",\n"));

    QStringList tableSpecs = this->specs(table);
    SQL += tableSpecs.join(",\n");

    SQL += ";\n";

    _connection->query(SQL);

    return true;
}

bool MySQLTableEditor::drop(EntityInDatabase * entity)
{
    class MySQLForeignKeyDisabler
    {
    public:
        explicit MySQLForeignKeyDisabler(Connection * connection) :
            _connection(connection)
        {
            if (_connection->serverVersionInt() >= 40014) {
                _connection->query(
                    "SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS,"
                    " FOREIGN_KEY_CHECKS=0");
            }
        }
        ~MySQLForeignKeyDisabler()
        {
            try {
                if (_connection->serverVersionInt() >= 40014) {
                    _connection->query(
                        "SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS");
                }
            } catch(meow::db::Exception & ex) {
                qDebug() << "Catched:" << ex.message();
            }
        }
    private:
        Connection * _connection;
    };

    QString SQL = dropSQL(entity);
    if (SQL.length()) {
        MySQLForeignKeyDisabler fKeysDisabler(_connection);
        (void)fKeysDisabler;

        _connection->query(SQL);
        return true;
    }

    return false;
}

void MySQLTableEditor::rename(TableEntity * table, const QString & newName)
{
    QString SQL = QString("RENAME TABLE %1 TO %2;")
            .arg(db::quotedName(table))
            .arg(_connection->quoteIdentifier(newName));

    _connection->query(SQL);
}

QString MySQLTableEditor::sqlCode(const TableColumn * column) const
{
    QString SQL = _connection->quoteIdentifier(column->name()) + " ";
    SQL += column->dataTypeName();

    if (column->lengthSet().length()
            && db::dataTypeHasLength(column->dataType())) {
        SQL += QString("(%1)").arg(column->lengthSet());
    }

    bool isNumericType = db::dataTypeIsNumeric(column->dataType());
    if (isNumericType) {
        if (column->isUnsigned()) {
            SQL += " UNSIGNED";
        }
        if (column->isZeroFill()) {
            SQL += " ZEROFILL";
        }
    }

    if (column->isAllowNull() == false) {
        SQL += " NOT";
    }
    SQL += " NULL";

    if (column->defaultType() != ColumnDefaultType::None) {
        QString defaultText = _connection->escapeString(column->defaultText());
        if (column->dataType() == DataTypeIndex::Bit) {
            defaultText = "b" + defaultText;
        }
        QString timeStampLen;
        if (column->lengthSet().length()) {
            timeStampLen = QString("(%1)").arg(column->lengthSet());
        }
        switch (column->defaultType()) {
        case ColumnDefaultType::Text:
            SQL += " DEFAULT " + defaultText;
            break;
        case ColumnDefaultType::TextUpdateTS:
            SQL += " DEFAULT " + defaultText;
            SQL += " ON UPDATE CURRENT_TIMESTAMP" + timeStampLen;
            break;
        case ColumnDefaultType::Null:
            SQL += " DEFAULT NULL";
            break;
        case ColumnDefaultType::NullUpdateTS:
            SQL += " DEFAULT NULL ON UPDATE CURRENT_TIMESTAMP";
            SQL += timeStampLen;
            break;
        case ColumnDefaultType::CurTS:
            SQL += " DEFAULT CURRENT_TIMESTAMP";
            SQL += timeStampLen;
            break;
        case ColumnDefaultType::CurTSUpdateTS:
            SQL += " DEFAULT CURRENT_TIMESTAMP" + timeStampLen;
            SQL += " ON UPDATE CURRENT_TIMESTAMP" + timeStampLen;
            break;
        case ColumnDefaultType::AutoInc:
            SQL += " AUTO_INCREMENT";
            break;
        default:
            break;
        }
    }

    if (column->comment().length()) {
        SQL += " COMMENT " + _connection->escapeString(column->comment());
    }

    if (column->collation().length()) {
        SQL += " COLLATE " + _connection->escapeString(column->collation());
    }

    return SQL;
}

QString MySQLTableEditor::sqlCode(TableIndex * index) const
{
    if (index->columnsCount() == 0) {
        return QString();
    }
    QString SQL;

    if (index->isPrimaryKey()) {
        SQL += "PRIMARY KEY ";
    } else {
        if (index->classType() != TableIndexClass::Key) {
            SQL += index->classTypeStr() + " ";
        }
        SQL += "INDEX " + _connection->quoteIdentifier(index->name());
    }

    SQL += " (";
    QStringList columnNames;
    for (const auto & column : index->columns()) {
        columnNames << _connection->quoteIdentifier(column.name());
    }
    SQL += columnNames.join(", ");
    SQL += ')';

    if (index->indexType() != TableIndexType::None) {
        SQL += " USING " + index->indexTypeStr();
    }

    return SQL;
}

QString MySQLTableEditor::alterColumnSQL(const QString & oldName,
                                         const QString & colSQL) const
{
    return QString("CHANGE COLUMN %1 %2").arg(oldName).arg(colSQL);
}

QString MySQLTableEditor::dropSQL(EntityInDatabase * entity) const
{
    if (entity->type() == Entity::Type::Table) {
        return QString("DROP TABLE %1").arg(db::quotedName(entity));
    }
    Q_ASSERT(0);
    return QString();
}

QString MySQLTableEditor::dropSQL(const TableIndex * index) const
{
    QString indexName;
    if (index->isPrimaryKey()) {
        indexName = "PRIMARY KEY";
    } else {
        indexName = "INDEX " + _connection->quoteIdentifier(index->name());
    }

    return QString("DROP %1").arg(indexName);
}

QStringList MySQLTableEditor::specs(TableEntity * table, TableEntity * newData)
{
    bool insert = newData == nullptr;

    TableEntityComparator diff;
    diff.setCurrTable(newData);
    diff.setPrevTable(table);

    QStringList specs;

    if (insert || diff.commentDiffers()) {
        QString comment = insert ? table->structure()->comment()
                                 : newData->structure()->comment();
        // TODO: skip if insert and comment is empty
        specs << "COMMENT=" + _connection->escapeString(comment);
    }

    if (insert) {
        // TODO
    } else {

        QString collation = newData->collation();
        QString engine = newData->engineStr();
        QString rowFormat = newData->structure()->rowFormatStr();

        if (diff.collateDiffers() && !collation.isEmpty()) {
            specs << "COLLATE="
                     + _connection->escapeString(collation);
        }
        if (diff.engineDiffers() && !engine.isEmpty()) {
            if (newData->connection()->serverVersionInt() < 40018) {
                specs << "TYPE=" + engine;
            } else {
                specs << "ENGINE=" + engine;
            }
        }
        if (diff.rowFormatDiffers() && !rowFormat.isEmpty()) {
            specs << "ROW_FORMAT=" + rowFormat;
        }
        if (diff.checksumDiffers()) {
            int checksum = newData->structure()->isCheckSum();
            specs << "CHECKSUM=" + QString::number(checksum);
        }
        if (diff.autoIncrementDiffers()) {
            db::ulonglong autoIncr = newData->structure()->autoInc();
            specs << "AUTO_INCREMENT=" + QString::number(autoIncr);
        }
        if (diff.avgRowLenDiffers()) {
            db::ulonglong avgRowLen = newData->structure()->avgRowLen();
            specs << "AVG_ROW_LENGTH=" + QString::number(avgRowLen);
        }
        if (diff.maxRowsDiffers()) {
            db::ulonglong maxRows = newData->structure()->maxRows();
            specs << "MAX_ROWS=" + QString::number(maxRows);
        }
    }

    return specs;
}

} // namespace db
} // namespace meow
