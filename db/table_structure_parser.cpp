#include "table_structure_parser.h"
#include "db/entity/table_entity.h"
#include "db/entity/session_entity.h"
#include "db/connection.h"
#include <algorithm>
#include <QDebug>

namespace meow {
namespace db {

ITableStructureParser::ITableStructureParser(Connection * connection)
    :_connection(connection)
{

}

ITableStructureParser::~ITableStructureParser() {}

// ---------------------------------------------------------------

TableStructureParser::TableStructureParser(Connection * connection)
    :ITableStructureParser(connection),
     _wasInit(false),
     _charsetRegexp(nullptr),
     _collateRegexp(nullptr),
     _defaultCurTSRegexp(nullptr),
     _defaultOnUpdCurTSRegexp(nullptr),
     _firstWordRegexp(nullptr),
     _indiciesKeysRegexp(nullptr),
     _foreignKeysRegexp(nullptr),
     _tableOptionsRegexp(nullptr)
{

}

TableStructureParser::~TableStructureParser()
{
    // TODO std::unique_ptr's
    delete _charsetRegexp;
    delete _collateRegexp;
    delete _defaultCurTSRegexp;
    delete _defaultOnUpdCurTSRegexp;
    delete _firstWordRegexp;
    delete _indiciesKeysRegexp;
    delete _foreignKeysRegexp;
    delete _tableOptionsRegexp;
}

void TableStructureParser::run(TableEntity * table)
{
    // TODO: using regexps for parsing is very shitty

    init(table);

    // TODO: run parser for MySQL only
    // For PG it's stupid: we generate create code and then parse it
    // while we can just get all data from SQL queries

    QString createSQL = table->createCode();

    parseColumns(createSQL, table); // keep first
    parseKeysIndicies(createSQL, table);
    parseForeignKeys(createSQL, table);
    parseTableOptions(table);
}

void TableStructureParser::init(TableEntity * table)
{
    if (_wasInit) return;
    _wasInit = true;

    prepareTypes();

    _quoteChar = table->connection()->getIdentQuote();

    // many thanks to this site: https://regex101.com/ :)

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

    QString quotesStr = _quoteChar;
    QString keysRegexpStr1 =
        QString(R"(^\s+((\w+)\s+)?KEY\s+([%1]?([^%1]+)[%1]?\s+)?)").arg(quotesStr);
    QString keysRegexpStr2 =
        QString(R"(((USING|TYPE)\s+(\w+)\s+)?\((.+)\)(\s+USING\s+(\w+))?(\s+KEY_BLOCK_SIZE(\s|\=)+\d+)?,?$)");

    // CONSTRAINT `FK1` FOREIGN KEY (`which`)
    // REFERENCES `fk1` (`id`)
    // ON DELETE SET NULL ON UPDATE CASCADE
    QString fKeysRegexpStrFK =
        QString(R"(\s+CONSTRAINT\s+[%1]([^%1]+)[%1]\sFOREIGN KEY\s+\(([^\)]+)\))").arg(quotesStr);
    QString fKeysRegexpStrRef =
        QString(R"(\s+REFERENCES\s+[%1]([^\(]+)[%1]\s\(([^\)]+)\))").arg(quotesStr);
    QString fKeysRegexpStrRefOnDel =
        QString(R"((\s+ON DELETE (RESTRICT|CASCADE|SET NULL|NO ACTION|SET DEFAULT))?)");
    QString fKeysRegexpStrRefOnUpd =
        QString(R"((\s+ON UPDATE (RESTRICT|CASCADE|SET NULL|NO ACTION|SET DEFAULT))?)");

    _defaultCurTSRegexp = new QRegularExpression(defaultRegexpStrCurTS,
                          QRegularExpression::CaseInsensitiveOption);
    _defaultOnUpdCurTSRegexp = new QRegularExpression(defaultRegexpStrOnUpdCurTs,
                               QRegularExpression::CaseInsensitiveOption);
    _firstWordRegexp = new QRegularExpression(firstWordRegexp);
    _indiciesKeysRegexp = new QRegularExpression(keysRegexpStr1 + keysRegexpStr2,
        QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);

    _foreignKeysRegexp = new QRegularExpression(
        fKeysRegexpStrFK + fKeysRegexpStrRef + fKeysRegexpStrRefOnDel + fKeysRegexpStrRefOnUpd,
        QRegularExpression::CaseInsensitiveOption);

    QString optionsPairsRegexp =
        QString(R"(\s(\S+)\s*=\s*(\S+))");
    _tableOptionsRegexp = new QRegularExpression(
        optionsPairsRegexp,
        QRegularExpression::CaseInsensitiveOption);

}

void TableStructureParser::parseColumns(const QString & createSQL,
                                        TableEntity * table) const
{

    TableStructure * structure = table->structure();

    structure->clearColumns();

    // CREATE TABLE `address` (
    //     `phone` VARCHAR(20) NOT NULL, #1


    QString quotesRegexpStr = QString("[") + _quoteChar + "]"; // TODO: H:QuoteRegExprMetaChars
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

        structure->appendColumn(column);

        //qDebug() << column->operator QString();
    }
}

void TableStructureParser::parseKeysIndicies(
    const QString & createSQL,
    TableEntity * table) const
{

    table->structure()->removeAllIndicies();

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

        TableIndex * index = new TableIndex(table);
        index->setName(keyName);
        index->setClassType(indexClassStr);
        if (index->classType() == TableIndexClass::None) {
            index->setClassType(TableIndexClass::Key);
        }
        index->setIndexType(indexTypeStr);
        QStringList columnNames = explodeQuotedList(indexColumns);
        for (const auto & columnName : columnNames) {
            index->addColumn(columnName);
        }

        // TODO: subparts

        //qDebug() << index->operator QString();

        table->structure()->appendIndex(index);
    }
}

void TableStructureParser::parseForeignKeys(
    const QString & createSQL,
    TableEntity * table) const
{

    QList<ForeignKey *> & fKeys = table->structure()->foreignKeys();

    qDeleteAll(fKeys);
    fKeys.clear();

    auto regExpIt = _foreignKeysRegexp->globalMatch(createSQL);

    while (regExpIt.hasNext()) {
        QRegularExpressionMatch keyMatch = regExpIt.next();

        QString keyName = keyMatch.captured(1);
        QString columnsStr = keyMatch.captured(2);
        QString refTable = keyMatch.captured(3);
        QString fColumnsStr = keyMatch.captured(4);
        QString onDeleteStr = keyMatch.captured(6);
        QString onUpdateStr = keyMatch.captured(8);

        ForeignKey * fKey = new ForeignKey(table);
        fKey->setName(keyName);
        fKey->setReferenceTable(refTable);
        QStringList columnNames = explodeQuotedList(columnsStr);
        fKey->setColumns(columnNames);
        fKey->referenceColumns() = explodeQuotedList(fColumnsStr);
        fKey->setOnDelete(onDeleteStr);
        fKey->setOnUpdate(onUpdateStr);

        fKeys.append(fKey);

    }
}

void TableStructureParser::parseTableOptions(TableEntity * table)
{
    QString comment;
    QString engineStr;
    QString collation;
    QString rowFormatStr;
    db::ulonglong avgRowLen = 0;
    db::ulonglong autoInc = 0;
    db::ulonglong maxRows = 0;
    bool isCheckSum = false;

    TableStructure * structure = table->structure();
    QString createSQL = table->createCode();

    auto regExpIt = _tableOptionsRegexp->globalMatch(createSQL);

    // TODO we already read some data like Engine, Collation in
    // Entities fetcher (SHOW TABLE STATUS)

    while (regExpIt.hasNext()) {
        QRegularExpressionMatch optionMatch = regExpIt.next();
        QString optName = optionMatch.captured(1).toUpper();
        QString optValue = optionMatch.captured(2);
        if (optName == "COMMENT") {
            comment = table->connection()->unescapeString(matchQuotedStr(optValue));
        } else if (optName == "ENGINE" || optName == "TYPE") {
            engineStr = optValue;
        } else if (optName == "COLLATE") {
            collation = optValue;
        } else if (optName == "AVG_ROW_LENGTH") {
            avgRowLen = optValue.toULongLong();
        } else if (optName == "AUTO_INCREMENT") {
            autoInc = optValue.toULongLong();
        } else if (optName == "ROW_FORMAT") {
            rowFormatStr = optValue;
        } else if (optName == "CHECKSUM") {
            isCheckSum = optValue == "1";
        } else if (optName == "MAX_ROWS") {
            maxRows = optValue.toULongLong();
        } // TODO "INSERT_METHOD", "UNION"

    }

    table->setEngine(engineStr);
    if (collation.length()) {
        table->setCollation(collation);
    }

    // Put additional (info-like) attributes into structure
    structure->setComment(comment);
    structure->setAvgRowLen(avgRowLen);
    structure->setAutoInc(autoInc);
    structure->setRowFormat(rowFormatStr);
    structure->setCheckSum(isCheckSum);
    structure->setMaxRows(maxRows);
}

QString TableStructureParser::extractId(QString & str, bool remove) const
{
    const int leftPos = str.indexOf(_quoteChar, 0, Qt::CaseInsensitive) + 1;

    if (leftPos > 0) {
        const int rightPos =
            str.indexOf(_quoteChar, leftPos, Qt::CaseInsensitive) - 1;
        if (rightPos > leftPos) {
            QString result = str.mid(leftPos, rightPos - leftPos + 1);
            if (remove) {
                str.remove(0, rightPos + 2);
            }
            return result;
        }
    }
    return QString();
}

DataTypePtr TableStructureParser::extractDataTypeByName(
        QString & columnString) const
{

    int startPos = 0;
    int len = columnString.length();
    while (startPos < len && columnString.at(startPos).isSpace()) {
        ++startPos;
    }
    // assume _typesSorted is sorted by type name len desc,
    // so we match longest first

    for (const DataTypePtr & type : _typesSorted) {
        QString typeName = type->name;
        QStringRef startRef = columnString.midRef(startPos, typeName.length());
        if (QString::compare(typeName, startRef, Qt::CaseInsensitive) == 0) {
            columnString.remove(0, startPos + typeName.length());
            return type;
        }
    }

    return std::make_shared<DataType>();
}

void TableStructureParser::prepareTypes()
{
    if (!_typesSorted.empty()) return;

    _typesSorted = _connection->dataTypes()->list().toStdList();

    _typesSorted.sort(
        [=](const DataTypePtr & l, const DataTypePtr & r) {
            return l->name.length() > r->name.length(); // the longest first
        }
    );
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
        if ((closeIndex - 1) > startPos) { // TODO: always false!
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
    // TODO: this is for MySQL only: move to separate class

    // AUTO_INCREMENT
    // DEFAULT NULL,
    // DEFAULT '19.99'
    // DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,

    bool isAutoIncrement = isStartsFromString(columnString, "AUTO_INCREMENT");

    // PG-only, remove to PG-related class
    if (!isAutoIncrement) {
        isAutoIncrement = isStartsFromString(columnString, "DEFAULT nextval(");
    }

    if (isAutoIncrement) {
        column->setDefaultType(ColumnDefaultType::AutoInc);
        //column->setDefaultText("AUTO_INCREMENT");
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
            //column->setDefaultText("NULL");
            if (checkForOnUpdateCurTs(columnString)) {
                column->setDefaultType(ColumnDefaultType::NullUpdateTS);
            }
            return columnString;
        }
        QRegularExpressionMatch matchCurTS = _defaultCurTSRegexp->match(columnString);
        if (matchCurTS.hasMatch()) {
            columnString.remove(0, matchCurTS.capturedLength(0));
            column->setDefaultType(ColumnDefaultType::CurTS);
            //column->setDefaultText("CURRENT_TIMESTAMP");
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

QStringList TableStructureParser::explodeQuotedList(QString & str) const
{
    QStringList result;
    QStringList rawRes = str.split(',');
    for (auto & rawItem : rawRes) {
        QString item = extractId(rawItem);
        if (item.length()) {
            result.append(item);
        }
    }
    return result;
}

} // namespace db
} // namespace meow

