#ifndef DB_VIEW_STRUCTURE_H
#define DB_VIEW_STRUCTURE_H

#include <QString>
#include "table_column.h"

namespace meow {
namespace db {

class ViewEntity;

// Intent: holds view options/fields
class ViewStructure
{
public:
    ViewStructure(ViewEntity * view);

    void setSelectStatement(const QString & select) {
        _selectStatement = select;
    }
    const QString & selectStatement() const {
        return _selectStatement;
    }

    void clear() {
        _selectStatement = QString();
        algorithm = QString();
        definer = QString();
        sqlSecurity = QString();
        checkOption = QString();
        clearColumns();
    }

    const QList<TableColumn *> & columns() const { return _columns; }
    void clearColumns() { qDeleteAll(_columns); _columns.clear(); }
    void appendColumn(TableColumn * column) { _columns.append(column); }

    ViewStructure * deepCopy(ViewEntity * parentView) const;

    bool operator==(const ViewStructure & other);

private:
    ViewEntity * _view; // TODO: need?
    QString _selectStatement;

public: // I'm lazy

    // MySQL Specific -> move to descendant
    QString algorithm;
    QString definer;
    QString sqlSecurity;
    QString checkOption;

    QList<TableColumn *> _columns;
};

} // namespace db
} // namespace meow

#endif // DB_VIEW_STRUCTURE_H
