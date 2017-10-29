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

class TableStructureParser
{
public:
    TableStructureParser();
    ~TableStructureParser();
    void run(TableEntity * table);
private:
    void parseColumns(const QString & createSQL, QList<TableColumn *> & columns) const;
    QString extractId(QString & columnString) const;
    DataTypeIndex extractDataTypeByName(QString & columnString) const;
    void prepareTypes();
    QString extractLengthSet(QString & columnString) const;
    bool isStartsFromString(QString & columnString, const QString & needle) const;
    void init();
    QString extractCharset(QString & columnString) const;
    QString extractCollate(QString & columnString) const;
    bool detectAllowNull(QString & columnString) const;
    void parseDefault(QString & columnString, TableColumn * column) const;
    bool checkForOnUpdateCurTs(QString & columnString) const;

    QList<QPair<QString, DataTypeIndex>> _types;
    bool _wasInit;
    QRegularExpression * _charsetRegexp;
    QRegularExpression * _collateRegexp;
    QRegularExpression * _defaultCurTSRegexp;
    QRegularExpression * _qoutedStrRegexp;
    QRegularExpression * _defaultOnUpdCurTSRegexp;
    QRegularExpression * _firstWordRegexp;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_PARSER_H
