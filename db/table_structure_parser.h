#ifndef DB_TABLE_STRUCTURE_PARSER_H
#define DB_TABLE_STRUCTURE_PARSER_H

#include <QList>
#include <QPair>
#include "db/data_type/data_type.h"
#include <QRegularExpression>

namespace meow {
namespace db {

class TableEntity;
class TableColumn;
class TableIndex;
class ForeignKey;

class TableStructureParser
{
public:
    TableStructureParser();
    ~TableStructureParser();
    void run(TableEntity * table);
private:
    void parseColumns(const QString & createSQL, QList<TableColumn *> & columns) const;
    void parseKeysIndicies(const QString & createSQL, QList<TableIndex *> & indicies) const;

    QString extractId(QString & columnString) const;
    DataTypeIndex extractDataTypeByName(QString & columnString) const;
    void prepareTypes();
    QString extractLengthSet(QString & columnString) const;
    bool isStartsFromString(QString & columnString, const QString & needle) const;
    void init();
    QString extractCharset(QString & columnString) const;
    QString extractCollate(QString & columnString) const;
    bool detectAllowNull(QString & columnString) const;
    QString parseDefault(QString & columnString, TableColumn * column) const;
    bool checkForOnUpdateCurTs(QString & columnString) const;
    QString matchQuotedStr(QString & columnString) const;
    QString parseComment(QString & columnString, TableColumn * column) const;

    QList<QPair<QString, DataTypeIndex>> _types;
    bool _wasInit;
    QRegularExpression * _charsetRegexp;
    QRegularExpression * _collateRegexp;
    QRegularExpression * _defaultCurTSRegexp;
    QRegularExpression * _defaultOnUpdCurTSRegexp;
    QRegularExpression * _firstWordRegexp;
    QRegularExpression * _indiciesKeysRegexp;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_PARSER_H
