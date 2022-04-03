#ifndef DB_TABLE_STRUCTURE_PARSER_H
#define DB_TABLE_STRUCTURE_PARSER_H

#include <QList>
#include <QPair>
#include "db/data_type/data_type.h"
#include <QRegularExpression>

namespace meow {
namespace db {

class TableEntity;
class Connection;

class ITableStructureParser
{
public:
    explicit ITableStructureParser(Connection * connection);
    virtual ~ITableStructureParser();
    virtual void run(TableEntity * table) = 0;
    virtual DataTypePtr extractDataTypeByName(QString & columnString) = 0;
    virtual QString extractLengthSet(QString & columnString) = 0;
protected:
    Connection * _connection;
};


class TableColumn;
class TableIndex;
class ForeignKey;


// TODO: make specific for each DBMS
class TableStructureParser : public ITableStructureParser
{
public:
    explicit TableStructureParser(Connection * connection);
    virtual ~TableStructureParser() override;
    virtual void run(TableEntity * table) override;
    virtual DataTypePtr extractDataTypeByName(QString & columnString) override;
    virtual QString extractLengthSet(QString & columnString) override;

private:
    void parseColumns(const QString & createSQL, TableEntity * table);
    void parseKeysIndicies(const QString & createSQL, TableEntity * table) const;
    void parseForeignKeys(const QString & createSQL, TableEntity * table) const;
    void parseTableOptions(TableEntity * table);

    QString extractId(QString & str, bool remove = true) const;
    void prepareTypes();
    bool isStartsFromString(QString & columnString, const QString & needle) const;
    void init(TableEntity * table);
    QString extractCharset(QString & columnString) const;
    QString extractCollate(QString & columnString) const;
    bool detectAllowNull(QString & columnString) const;
    QString parseDefault(QString & columnString, TableColumn * column) const;
    bool checkForOnUpdateCurTs(QString & columnString) const;
    QString matchQuotedStr(QString & columnString) const;
    QString parseComment(QString & columnString, TableColumn * column) const;
    QStringList explodeQuotedList(QString & str) const;

    std::list<DataTypePtr> _typesSorted;
    bool _wasInit;
    QRegularExpression * _charsetRegexp;
    QRegularExpression * _collateRegexp;
    QRegularExpression * _defaultCurTSRegexp;
    QRegularExpression * _defaultOnUpdCurTSRegexp;
    QRegularExpression * _firstWordRegexp;
    QRegularExpression * _indiciesKeysRegexp;
    QRegularExpression * _foreignKeysRegexp;
    QRegularExpression * _tableOptionsRegexp;
    QChar _quoteChar;
};

} // namespace db
} // namespace meow

#endif // DB_TABLE_STRUCTURE_PARSER_H
