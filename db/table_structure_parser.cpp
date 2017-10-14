#include "table_structure_parser.h"
#include <QRegularExpression>
#include <QDebug>

namespace meow {
namespace db {

TableStructureParser::TableStructureParser()
{
}

void TableStructureParser::run(TableEntity * table)
{

    TableStructure * structure = table->structure();
    QString createSQL = table->createCode();

    parseColumns(createSQL, structure->columns());
}

void TableStructureParser::parseColumns(const QString & createSQL,
                                        QList<TableColumn *> & columns)
{
    columns.clear();

    // CREATE TABLE `address` (
    //     `phone` VARCHAR(20) NOT NULL, #1

    QString quotesRegexpStr = "[`]"; // TODO: mysql specific!
    QString columnLineRegexpStr = QString(
        R"(^\s+(%1.+),?$)" // #1
    ).arg(quotesRegexpStr);
    QRegularExpression columnLineRegexp =
        QRegularExpression(columnLineRegexpStr, QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator regExpIt = columnLineRegexp.globalMatch(createSQL);

    while (regExpIt.hasNext()) {
        QRegularExpressionMatch columnMatch = regExpIt.next();
        QString columnString = columnMatch.captured(1);
        //qDebug() << columnString;

        TableColumn * column = new TableColumn();
        columns.append(column);

        column->setName(extractId(columnString));
        //qDebug() << column->operator QString();
    }
}

QString TableStructureParser::extractId(QString & columnString)
{
    const QChar quote('`');

    const int leftPos = columnString.indexOf(quote, 0, Qt::CaseInsensitive) + 1;

    if (leftPos > 0) {
        const int rightPos =
            columnString.indexOf(quote, leftPos, Qt::CaseInsensitive) - 1;
        if (rightPos > leftPos) {
            return columnString.mid(leftPos, rightPos - leftPos + 1);
        }
    }
    return QString();
}

} // namespace db
} // namespace meow

