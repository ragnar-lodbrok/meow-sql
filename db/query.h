#ifndef DB_QUERY_H
#define DB_QUERY_H

#include <QStringList>
#include <vector>
#include <QMap>

#include "common.h"
#include "query_column.h"
#include "native_query_result_interface.h"

namespace meow {
namespace db {

class Connection;
class EditableGridData;
class Entity;

class Query
{
public:
    Query(Connection * connection = nullptr);
    virtual ~Query();

    void setSQL(const QString & SQL);
    void setConnection(Connection * connection) { _connection = connection; }

    const QString & SQL() const { return _SQL; }
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

    // H: procedure Execute(AddResult: Boolean=False; UseRawResult: Integer=-1); virtual; abstract;
    virtual void execute(bool addResult = false) = 0;

    virtual bool hasResult() = 0;

    virtual void seekRecNo(db::ulonglong value) = 0; // H: SetRecNo

    virtual QString curRowColumn(std::size_t index, bool ignoreErrors = false) = 0;

    QString curRowColumn(const QString & colName, bool ignoreErrors = false);

    QStringList curRow();

    QMap<QString, QString> curRowAsObject();

    virtual bool isNull(std::size_t index) = 0; // TODO: add by name mthd

    // true if was already prepared
    virtual bool prepareEditing();
    bool isEditing() const { return _editableData != nullptr; }
    EditableGridData * editableData() const {
        return _editableData;
    }

    void seekFirst();
    void seekNext();

    std::size_t indexOfColumn(const QString & colName) const;

    Entity * entity() const { return _entity; }
    void setEntity(Entity * entity) { _entity = entity; }

    QStringList keyColumns() const;

protected:

    db::ulonglong _recordCount;
    db::ulonglong  _curRecNo; // H: FRecNo
    std::vector<QueryColumn> _columns;
    QMap<QString, std::size_t> _columnIndexes; // Column name -> column index
    bool _eof; // H: FEof
    EditableGridData * _editableData; // TODO: move to QueryData?

private:
    QString _SQL;
    Connection * _connection;
    Entity * _entity;
};

} // namespace db
} // namespace meow

#endif // DB_QUERY_H
