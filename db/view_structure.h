#ifndef DB_VIEW_STRUCTURE_H
#define DB_VIEW_STRUCTURE_H

#include <QString>

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
    }

private:
    ViewEntity * _view; // TODO: need?
    QString _selectStatement;

public: // I'm lazy

    // MySQL Specific -> move to descendant
    QString algorithm;
    QString definer;
    QString sqlSecurity;
    QString checkOption;
};

} // namespace db
} // namespace meow

#endif // DB_VIEW_STRUCTURE_H
