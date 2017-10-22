#include "table_structure_parser.h"
#include "db/entity/table_entity.h"
#include <QDebug>
#include <algorithm>

namespace meow {
namespace db {

TableStructureParser::TableStructureParser()
    :_wasInit(false),
     _charsetRegexp(nullptr),
     _collateRegexp(nullptr)
{

}

TableStructureParser::~TableStructureParser()
{
    delete _charsetRegexp;
    delete _collateRegexp;
}

void TableStructureParser::run(TableEntity * table)
{
    init();

    TableStructure * structure = table->structure();
    QString createSQL = table->createCode();

    parseColumns(createSQL, structure->columns());
}

void TableStructureParser::init()
{
    if (_wasInit) return;
    _wasInit = true;

    prepareTypes();

    QString charsetRegexpStr = QString(R"(^CHARACTER SET (\w+)\b\s*)");
    _charsetRegexp = new QRegularExpression(charsetRegexpStr,
                     QRegularExpression::CaseInsensitiveOption);

    QString collateRegexpStr = QString(R"(^COLLATE (\w+)\b\s*)");
    _collateRegexp = new QRegularExpression(collateRegexpStr,
                     QRegularExpression::CaseInsensitiveOption);
}

void TableStructureParser::parseColumns(const QString & createSQL,
                                        QList<TableColumn *> & columns) const
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

        //qDebug () << columnString;

        TableColumn * column = new TableColumn();

        column->setName(extractId(columnString));
        if (column->name().isEmpty()) continue;

        column->setDataType(extractDataTypeByName(columnString));
        if (column->dataType() == DataTypeIndex::None) continue;

        column->setLengthSet(extractLengthSet(columnString));

        column->setIsUnsigned(
            isStartsFromString(columnString, QString("UNSIGNED")));

        column->setIsZeroFill(
            isStartsFromString(columnString, QString("ZEROFILL")));

        columnString = columnString.trimmed();
        column->setCharset(extractCharset(columnString));

        columnString = columnString.trimmed();
        column->setCollation(extractCollate(columnString));

        // TODO: default collation when no collate
        // TODO: virtual columns

        columnString = columnString.trimmed();
        column->setAllowNull(detectAllowNull(columnString));

        //qDebug() << "a:" << columnString;

        columns.append(column);

        //qDebug() << column->operator QString();
    }
}

QString TableStructureParser::extractId(QString & columnString) const
{
    const QChar quote('`');

    const int leftPos = columnString.indexOf(quote, 0, Qt::CaseInsensitive) + 1;

    if (leftPos > 0) {
        const int rightPos =
            columnString.indexOf(quote, leftPos, Qt::CaseInsensitive) - 1;
        if (rightPos > leftPos) {
            QString result = columnString.mid(leftPos, rightPos - leftPos + 1);
            columnString.remove(0, rightPos + 2);
            return result;
        }
    }
    return QString();
}

DataTypeIndex TableStructureParser::extractDataTypeByName(
    QString & columnString) const
{


    int startPos = 0;
    int len = columnString.length();
    while (startPos < len && columnString.at(startPos).isSpace()) {
        ++startPos;
    }
    // assume _types is sorted by type name len desc, so we match longest first
    for (int i = 0; i < _types.size(); ++i) {
        const QPair<QString, DataTypeIndex> & typePair = _types.at(i);
        QString typeName = typePair.first;
        QStringRef startRef = columnString.midRef(startPos, typeName.length());
        if (QString::compare(typeName, startRef, Qt::CaseInsensitive) == 0) {
            columnString.remove(0, startPos + typeName.length());
            return typePair.second;
        }
    }

    return DataTypeIndex::None;
}

void TableStructureParser::prepareTypes()
{
    if (!_types.empty()) return;

    const QMap<DataTypeIndex, QString> & typesMap = dataTypeNames();
    auto names = typesMap.values().toStdList();
    names.sort(
        [=](const QString & l, const QString & r) {
            return l.length() > r.length(); // the longest first
        }
    );
    for (const QString & typeName : names) {
        DataTypeIndex typeIndex = typesMap.key(typeName, DataTypeIndex::None);
        _types.append( {typeName , typeIndex} );
    }
}

QString TableStructureParser::extractLengthSet(QString & columnString) const
{

    QString res;
    int startPos = 0;
    int len = columnString.length();
    while (startPos < len && columnString.at(startPos).isSpace()) {
        ++startPos;
    }

    int closeIndex = startPos;

    if (startPos < columnString.length()
        && columnString.at(startPos) == QChar('(')) {

        bool inLiteral = false;

        for (int i = startPos; i < len; ++i) {
            if (columnString.at(i) == QChar(')') && !inLiteral) {
                closeIndex = i;
                break;
            }
            if (columnString.at(i) == QChar('\'')) {
                inLiteral = !inLiteral;
            }
        }
        if ((closeIndex - 1) > startPos) {
            res = columnString.mid(startPos + 1, closeIndex - startPos - 1);
        }
    }

    if (closeIndex > startPos) {
        columnString.remove(0, closeIndex + 1);
    } else if (startPos > 0) {
        columnString.remove(0, startPos);
    }

    return res;
}

bool TableStructureParser::isStartsFromString(
        QString & columnString,
        const QString & needle) const
{
    int startPos = 0;
    int len = columnString.length();
    while (startPos < len && columnString.at(startPos).isSpace()) {
        ++startPos;
    }

    QStringRef startRef = columnString.midRef(startPos, needle.length());
    if (QString::compare(needle, startRef, Qt::CaseInsensitive) == 0) {
        columnString.remove(0, startPos + needle.length());
        return true;
    }

    columnString.remove(0, startPos);
    return false;
}

QString TableStructureParser::extractCharset(QString & columnString) const
{
    QRegularExpressionMatch match = _charsetRegexp->match(columnString);
    if (match.hasMatch()) {
        QString entireMatch = match.captured(0);
        QString charsetMatch = match.captured(1);
        columnString.remove(0, entireMatch.length());
        return charsetMatch;
    }
    return QString();
}

QString TableStructureParser::extractCollate(QString & columnString) const
{
    QRegularExpressionMatch match = _collateRegexp->match(columnString);
    if (match.hasMatch()) {
        QString entireMatch = match.captured(0);
        QString collateMatch = match.captured(1);
        columnString.remove(0, entireMatch.length());
        return collateMatch;
    }
    return QString();
}

bool TableStructureParser::detectAllowNull(QString & columnString) const
{
    if (isStartsFromString(columnString, "NOT NULL")) {
        return false;
    } else {
        isStartsFromString(columnString, "NULL"); // to remove
        return true;
    }
}

} // namespace db
} // namespace meow

