#include "table_structure_parser.h"
#include "db/entity/table_entity.h"
#include <QDebug>
#include <algorithm>

namespace meow {
namespace db {

TableStructureParser::TableStructureParser()
    :_wasInit(false),
     _charsetRegexp(nullptr),
     _collateRegexp(nullptr),
     _defaultCurTSRegexp(nullptr),
     _defaultOnUpdCurTSRegexp(nullptr),
     _firstWordRegexp(nullptr),
     _indiciesKeysRegexp(nullptr)
{

}

TableStructureParser::~TableStructureParser()
{
    delete _charsetRegexp;
    delete _collateRegexp;
    delete _defaultCurTSRegexp;
    delete _defaultOnUpdCurTSRegexp;
    delete _firstWordRegexp;
    delete _indiciesKeysRegexp;
}

void TableStructureParser::run(TableEntity * table)
{
    init();

    TableStructure * structure = table->structure();
    QString createSQL = table->createCode();

    parseColumns(createSQL, structure->columns());
    parseKeysIndicies(createSQL, structure->indicies());
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

    // DEFAULT CURRENT_TIMESTAMP(6) ON UPDATE CURRENT_TIMESTAMP(6)
    QString defaultRegexpStrCurTS = R"(CURRENT_TIMESTAMP(\(\d+\))?)";
    QString defaultRegexpStrOnUpdCurTs = R"(\s+ON\s+UPDATE\s+CURRENT_TIMESTAMP(\(\d+\))?)";
    QString firstWordRegexp = R"(^(\w+))";

    QString quotesStr = "`";
    QString keysRegexpStr1 =
        QString(R"(^\s+((\w+)\s+)?KEY\s+([%1]?([^%1]+)[%1]?\s+)?)").arg(quotesStr);
    QString keysRegexpStr2 =
        QString(R"(((USING|TYPE)\s+(\w+)\s+)?\((.+)\)(\s+USING\s+(\w+))?(\s+KEY_BLOCK_SIZE(\s|\=)+\d+)?,?$)");

    _defaultCurTSRegexp = new QRegularExpression(defaultRegexpStrCurTS,
                          QRegularExpression::CaseInsensitiveOption);
    _defaultOnUpdCurTSRegexp = new QRegularExpression(defaultRegexpStrOnUpdCurTs,
                               QRegularExpression::CaseInsensitiveOption);
    _firstWordRegexp = new QRegularExpression(firstWordRegexp);
    _indiciesKeysRegexp = new QRegularExpression(keysRegexpStr1 + keysRegexpStr2,
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
}

void TableStructureParser::parseColumns(const QString & createSQL,
                                        QList<TableColumn *> & columns) const
{
    columns.clear();

    // CREATE TABLE `address` (
    //     `phone` VARCHAR(20) NOT NULL, #1

    QString quotesRegexpStr = "[`]"; // TODO: mysql specific!
    QString columnLineRegexpStr = QString( // TODO: create regexp once
        R"(^\s+(%1.+),?$)" // #1
    ).arg(quotesRegexpStr);
    QRegularExpression columnLineRegexp =
        QRegularExpression(columnLineRegexpStr, QRegularExpression::MultilineOption);

    QRegularExpressionMatchIterator regExpIt = columnLineRegexp.globalMatch(createSQL);

    while (regExpIt.hasNext()) {
        QRegularExpressionMatch columnMatch = regExpIt.next();
        QString columnString = columnMatch.captured(1);

        TableColumn * column = new TableColumn();

        column->setName(extractId(columnString));
        if (column->name().isEmpty()) {
            delete column;
            continue;
        }

        column->setDataType(extractDataTypeByName(columnString));
        if (column->dataType() == DataTypeIndex::None) {
            delete column;
            continue;
        }

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

        columnString = parseDefault(columnString, column);

        columnString = parseComment(columnString, column);

        columns.append(column);

        //qDebug() << column->operator QString();
    }
}

void TableStructureParser::parseKeysIndicies(
    const QString & createSQL,
    QList<TableIndex *> & indicies) const
{
    auto regExpIt = _indiciesKeysRegexp->globalMatch(createSQL);

    while (regExpIt.hasNext()) {
        QRegularExpressionMatch keyMatch = regExpIt.next();
        QString keyName = keyMatch.captured(4);
        QString indexClassStr = keyMatch.captured(2);
        QString indexTypeStr;
        if ( ! keyMatch.captured(6).isEmpty()) {
            indexTypeStr = keyMatch.captured(7);
        } else {
            indexTypeStr = keyMatch.captured(10);
        }
        QString indexColumns = keyMatch.captured(8);

        if (keyName.isEmpty()) {
            keyName = indexClassStr;
        }
        if (keyName.isEmpty()) continue;

        TableIndex * index = new TableIndex;
        index->setName(keyName);
        index->setClassType(indexClassStr);
        if (index->classType() == TableIndexClass::None) {
            index->setClassType(TableIndexClass::Key);
        }
        index->setIndexType(indexTypeStr);
        index->columns() = indexColumns.split(',', QString::SkipEmptyParts);

        // TODO: subparts

        //qDebug() << index->operator QString();

        indicies.append(index);
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

QString TableStructureParser::parseDefault(
        QString & columnString,
        TableColumn * column) const
{
    // AUTO_INCREMENT
    // DEFAULT NULL,
    // DEFAULT '19.99'
    // DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

    bool isAutoIncrement = isStartsFromString(columnString, "AUTO_INCREMENT");

    if (isAutoIncrement) {
        column->setDefaultType(ColumnDefaultType::AutoInc);
        column->setDefaultText("AUTO_INCREMENT");
        return columnString;
    }

    bool hasDefault = isStartsFromString(columnString, "DEFAULT ");
    if (hasDefault == false) {
        return columnString;
    }

    bool inLiteral = columnString.startsWith("'")
                  || columnString.startsWith("b'")
                  || columnString.startsWith("('"); //?

    if (!inLiteral) {
        if (isStartsFromString(columnString, "NULL")) {
            column->setDefaultType(ColumnDefaultType::Null);
            column->setDefaultText("NULL");
            if (checkForOnUpdateCurTs(columnString)) {
                column->setDefaultType(ColumnDefaultType::NullUpdateTS);
            }
            return columnString;
        }
        QRegularExpressionMatch matchCurTS = _defaultCurTSRegexp->match(columnString);
        if (matchCurTS.hasMatch()) {
            columnString.remove(0, matchCurTS.capturedLength(0));
            column->setDefaultType(ColumnDefaultType::CurTS);
            column->setDefaultText("CURRENT_TIMESTAMP");
            if (checkForOnUpdateCurTs(columnString)) {
                column->setDefaultType(ColumnDefaultType::CurTSUpdateTS);
            }
            return columnString;
        }
        auto matchFirstWord = _firstWordRegexp->match(columnString);
        if (matchFirstWord.hasMatch()) {
            QString defaultText = matchFirstWord.captured(0);
            column->setDefaultType(ColumnDefaultType::Text);
            column->setDefaultText(defaultText);
            columnString.remove(0, matchFirstWord.capturedLength(0));
            if (checkForOnUpdateCurTs(columnString)) {
                column->setDefaultType(ColumnDefaultType::TextUpdateTS);
            }
            return columnString;
        }
    } else {
        columnString = columnString.trimmed();
        QString defaultText = matchQuotedStr(columnString);
        if (!defaultText.isNull()) {
            // TODO: unescape text
            column->setDefaultText(defaultText);
            column->setDefaultType(ColumnDefaultType::Text);
        }
        columnString = columnString.trimmed();
        if (checkForOnUpdateCurTs(columnString)) {
            column->setDefaultType(ColumnDefaultType::TextUpdateTS);
        }
        return columnString;
    }

    return columnString;
}

QString TableStructureParser::parseComment(
        QString & columnString,
        TableColumn * column) const
{
    if (isStartsFromString(columnString, "COMMENT")) {
        QString str = columnString.trimmed();
        QString commentStr = matchQuotedStr(str);
        if (!commentStr.isNull()) {
            column->setComment(commentStr);
        }
        return str;
    }

    return columnString;
}

QString TableStructureParser::matchQuotedStr(QString & str) const
{
    // 'string'
    // 'str''ing'
    // TODO: more eg escapes?

    int start = -1;
    int stop = -1;
    int len = str.length();
    bool inString = false;
    const QChar QUOTE = QChar('\'');

    for (int i=0; i<len; ++i) {
        const QChar curChar = str.at(i);
        if (curChar == QUOTE) {
            if (!inString) {
                inString = true;
                start = i;
            } else {
                const QChar nextChar = ((i+1) < len) ? str.at(i+1) : QChar();
                if (nextChar != QUOTE) {
                    stop = i;
                    break;
                } else {
                    i += 2;
                }
            }
        }
    }

    int matchLen = stop - start - 1;
    if (matchLen >= 0) {
         QString match = str.mid(start + 1, matchLen);
         str.remove(0, stop + 1);
         return match;
    }

    return QString();
}

bool TableStructureParser::checkForOnUpdateCurTs(QString & columnString) const
{
    auto match = _defaultOnUpdCurTSRegexp->match(columnString);
    if (match.hasMatch()) {
        columnString.remove(0, match.capturedLength(0));
        return true;
    }
    return false;
}

} // namespace db
} // namespace meow

