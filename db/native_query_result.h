#ifndef DB_NATIVE_QUERY_RESULT_INTERFACE_H
#define DB_NATIVE_QUERY_RESULT_INTERFACE_H

#include <memory>
#include <vector>
#include <QMap>
#include <QStringList>
#include "query_column.h"
#include "db/common.h"

namespace meow {
namespace db {

class EditableGridData;
class Entity;
class Connection;
class NativeQueryResult;
class ForeignKey;

using QueryResultPt = std::shared_ptr<NativeQueryResult>;

// Intent: represents native result of query like MYSQL_RES
class NativeQueryResult
{
public:

    NativeQueryResult(Connection * connection = nullptr);

    virtual ~NativeQueryResult();

    virtual db::ulonglong nativeRowsCount() const = 0;

    void setConnection(Connection * connection) {
        _connection = connection;
    }

    Connection * connection() const { return _connection; }

    db::ulonglong recordCount() const;
    bool isEof() const { return _eof; }

    std::size_t columnCount() const { return _columns.size(); }

    QString columnName(std::size_t index) const { return _columns[index].name; }

    QStringList columnOrgNames() const {
        QStringList names;
        for (const QueryColumn & col : _columns) {
            names << col.orgName;
        }
        return names;
    }

    QueryColumn & column(std::size_t index) { return _columns[index]; }

    virtual void seekRecNo(db::ulonglong value) = 0; // H: SetRecNo

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) = 0;

    QString curRowColumn(const QString & colName,
                         bool ignoreErrors = false);

    QStringList curRow();

    QMap<QString, QString> curRowAsObject();

    virtual bool hasData() const {
        return !_appendedResults.empty();
    }

    virtual bool isNull(std::size_t index) = 0; // TODO: add by name mthd

    // true if was already prepared
    bool prepareEditing();
    bool isEditing() const { return _editableData != nullptr; }
    EditableGridData * editableData() const {
        return _editableData;
    }

    void seekFirst();
    void seekNext();

    std::size_t indexOfColumn(const QString & colName) const;

    Entity * entity() const { return _entity; }
    void setEntity(Entity * entity) {
        _entity = entity;
        for (const QueryResultPt & appendedResult : _appendedResults) {
            appendedResult->setEntity(entity);
        }
    }

    QStringList keyColumns() const;
    QList<std::size_t> primaryColumnIndices() const;

    QList<ForeignKey *> foreignKeysForColumn(std::size_t index);
    QStringList columnValuesList(std::size_t index) const;

    void appendResultData(const QueryResultPt & result);

protected:

    virtual void prepareResultForEditing(NativeQueryResult * result) = 0;

    void throwOnInvalidColumnIndex(std::size_t index);

    db::ulonglong _recordCount;
    db::ulonglong _curRecNo; // H: FRecNo
    std::vector<QueryColumn> _columns;
    QMap<QString, std::size_t> _columnIndexes; // Column name -> column index
    bool _eof; // H: FEof
    EditableGridData * _editableData; // TODO: move to QueryData?
    Connection * _connection;
    Entity * _entity;
    std::vector<QueryResultPt> _appendedResults;
};


} // namespace db
} // namespace meow

#endif // DB_NATIVE_QUERY_RESULT_INTERFACE_H
