#include "sqlite_table_structure_parser.h"
#include "utils/sql_parser/sqlite/sqlite_parser.h"
#include "db/entity/table_entity.h"
#include "db/data_type/sqlite_connection_datatypes.h"
#include <QDebug>

namespace meow {
namespace db {

ForeignKey::ReferenceOption parserActionToFKOption(
        meow::utils::sql_parser::SQLiteDoOnAction action)
{
    using Action = meow::utils::sql_parser::SQLiteDoOnAction;

    switch (action) {

    case Action::SetNull:
        return ForeignKey::ReferenceOption::SetNull;

    case Action::SetDefault:
        return ForeignKey::ReferenceOption::SetDefault;

    case Action::Cascade:
        return ForeignKey::ReferenceOption::Cascade;

    case Action::Restrict:
        return ForeignKey::ReferenceOption::Restrict;

    default:
        return ForeignKey::ReferenceOption::NoAction;
    }
}

void fillForeignKeyFromData(
        ForeignKey * fKey,
        meow::utils::sql_parser::SQLiteForeignData * fData)
{
    fKey->setReferenceTable(
                QString::fromStdString(fData->foreignTableName));

    QStringList referenceColumns;

    for (const std::string & c : fData->foreignColumnNames) {
        referenceColumns << QString::fromStdString(c);
    }

    fKey->referenceColumns() = referenceColumns;

    using Action = meow::utils::sql_parser::SQLiteAction;

    for (const auto & action : fData->actions) {
        if (action.action == Action::OnDelete) {
            fKey->setOnDelete(parserActionToFKOption(action.doOnAction));
        } else if (action.action == Action::OnUpdate) {
            fKey->setOnUpdate(parserActionToFKOption(action.doOnAction));
        }
    }
}

SQLiteTableStructureParser::SQLiteTableStructureParser(
        Connection * connection)
    :ITableStructureParser(connection)
{
    // Listening: Metallica - Fight Fire With Fire
}


void SQLiteTableStructureParser::run(TableEntity * table)
{

    TableStructure * structure = table->structure();

    structure->clearColumns(); // TODO: add clear method of structure
    structure->removeAllIndicies();
    structure->removeAllIndicies();


    utils::sql_parser::SQLiteParser parser;

    bool success = parser.parseCreateTable(table->createCode().toStdString());

    if (!success) {
        // TODO: log error
        return;
    }

    qDebug() << "\n\ncolumns: ";

    auto types = static_cast<SQLiteConnectionDataTypes *>(
                    _connection->dataTypes());

    using Column = meow::utils::sql_parser::SQLiteColumnPtr;
    using ColConstr = meow::utils::sql_parser::SQLiteColumnConstraintPtr;
    using ConstType = meow::utils::sql_parser::SQLiteColumnConstraint::Type;
    using DefaultColConstr
    = meow::utils::sql_parser::SQLiteDefaultColumnConstraint;
    using LiteralType = meow::utils::sql_parser::SQLiteLiteralValueType;
    using FKeyColConstr
        = meow::utils::sql_parser::SQLiteForeignKeyColumnConstraint;
    using TableConstr
        = meow::utils::sql_parser::SQLiteTableConstraint;
    using TablePKConstr
        = meow::utils::sql_parser::SQLiteTablePrimaryKeyConstraint;
    using TableFKConstr
        = meow::utils::sql_parser::SQLiteTableForeignKeyConstraint;
    using TableUniqueConst
        = meow::utils::sql_parser::SQLiteTableUniqueConstraint;

    const std::vector<Column> & columns = parser._parsedTable->columns();

    for (const Column & parsedCol : columns ) {
        TableColumn * column = new TableColumn();

        const QString columnName = QString::fromStdString(parsedCol->name());

        column->setName(columnName);

        DataTypePtr basicDataType = types->dataTypeByName(
                QString::fromStdString(parsedCol->type()));

        // make copy of data type and change type name to real
        auto colDataType = DataTypePtr(new DataType(*basicDataType));
        colDataType->name = QString::fromStdString(parsedCol->type());

        column->setDataType(colDataType);

        structure->appendColumn(column);

        const std::vector<ColConstr> & constraints = parsedCol->constraints();

        for (const ColConstr & constr : constraints) {
            if (constr->type() == ConstType::PrimaryKey) {

                TableIndex * index = new TableIndex(table);
                index->setClassType(TableIndexClass::PrimaryKey);

                if (constr->isAutoincrement()) {
                    column->setDefaultType(ColumnDefaultType::AutoInc);
                    //column->setDefaultText("AUTOINCREMENT");
                }

                index->addColumn(columnName);

                qDebug() << "\t" << index->operator QString();

                table->structure()->appendIndex(index);

            } else if (constr->type() == ConstType::NotNull) {

                column->setAllowNull(false);

            } else if (constr->type() == ConstType::Unique) {

                TableIndex * index = new TableIndex(table);
                index->setClassType(TableIndexClass::Unique);

                index->addColumn(columnName);

                qDebug() << "\t" << index->operator QString();

                table->structure()->appendIndex(index);

            } else if (constr->type() == ConstType::Default) {
                auto defaultC
                        = static_cast<const DefaultColConstr *>(constr.get());

                LiteralType parsedDefaultType = defaultC->defaultValue.type;

                ColumnDefaultType defType = ColumnDefaultType::None;
                QString defaultValue = QString::Null();

                switch (parsedDefaultType) {

                    case LiteralType::Null:
                        defType = ColumnDefaultType::Null;
                        break;

                    case LiteralType::CurrentTimestamp:
                        defType = ColumnDefaultType::CurTS;
                        break;

                    case LiteralType::Numeric:
                    case LiteralType::String:
                        defType = ColumnDefaultType::Text;
                        defaultValue = QString::fromStdString(
                                    defaultC->defaultValue.value);
                        break;

                    case LiteralType::CurrentTime:
                        defType = ColumnDefaultType::CurTime;
                        break;

                    case LiteralType::CurrentDate:
                        defType = ColumnDefaultType::CurDate;
                        break;

                    default:
                        defType = ColumnDefaultType::None;
                        break;
                }

                // column can have several default constraints
                // don't overwrite old
                // TODO: need to change TableColumn to accept NULL separately

                if (defType != ColumnDefaultType::None) {
                    column->setDefaultType(defType);
                }

                if (!defaultValue.isNull()) {
                    column->setDefaultText(defaultValue);
                }
            } else if (constr->type() == ConstType::ForeignKey) {

                auto fKeyC = static_cast<const FKeyColConstr *>(constr.get());

                ForeignKey * fKey = new ForeignKey(table);

                fKey->setColumns(QStringList(columnName));

                fillForeignKeyFromData(fKey, fKeyC->foreignData.get());

                qDebug() << "\t" << fKey->operator QString();

                structure->foreignKeys().append(fKey);
            }
        }

        qDebug() << column->operator QString();
    }

    qDebug() << "\nconstraints: ";

    for (const auto & parsedConstr : parser._parsedTable->constraints()) {
        if (parsedConstr->type == TableConstr::Type::PrimaryKey) {

            auto parsedPK = static_cast<const TablePKConstr *>(
                        parsedConstr.get());

            TableIndex * index = new TableIndex(table);
            index->setClassType(TableIndexClass::PrimaryKey);
            index->setName(QString::fromStdString(parsedPK->name));

            QStringList columns;
            for (const auto & cName : parsedPK->indexedColumnNames) {
                index->addColumn(QString::fromStdString(cName));
            }

            qDebug() << "\t" << index->operator QString();

            table->structure()->appendIndex(index);

        } else if (parsedConstr->type == TableConstr::Type::ForeignKey) {

            auto parsedFK = static_cast<const TableFKConstr *>(
                        parsedConstr.get());

            ForeignKey * fKey = new ForeignKey(table);

            fKey->setName(QString::fromStdString(parsedFK->name));

            QStringList columns;
            for (const auto & cName : parsedFK->columnNames) {
                columns << QString::fromStdString(cName);
            }

            fKey->setColumns(columns);

            fillForeignKeyFromData(fKey, parsedFK->foreignData.get());

            qDebug() << "\t" << fKey->operator QString();

            structure->foreignKeys().append(fKey);

        } else if (parsedConstr->type == TableConstr::Type::Unique) {

            auto parsedUnique = static_cast<const TableUniqueConst *>(
                        parsedConstr.get());

            TableIndex * index = new TableIndex(table);
            index->setClassType(TableIndexClass::Unique);
            index->setName(QString::fromStdString(parsedUnique->name));

            QStringList columns;
            for (const auto & cName : parsedUnique->indexedColumnNames) {
                index->addColumn(QString::fromStdString(cName));
            }

            qDebug() << "\t" << index->operator QString();

            table->structure()->appendIndex(index);

        }
    }
}


} // namespace db
} // namespace meow
