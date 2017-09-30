#ifndef FOREIGN_KEY_H
#define FOREIGN_KEY_H

#include <QString>
#include <QStringList>

namespace meow {
namespace db {

// Represents foreign key
class ForeignKey
{
public:

    enum class ReferenceOption {
        NoAction,  // NO ACTION
        Restrict,  // RESTRICT
        Cascade,   // CASCADE
        SetNull,   // SET NULL
        SetDefault // SET DEFAULT
    };

    ForeignKey();
private:
    QString _name;
    QString _referenceTable;
    ReferenceOption _onUpdate;
    ReferenceOption _onDelete;
    QStringList _columns;
    QStringList _referenceColumns;

};

} // namespace db
} // namespace meow

#endif // FOREIGN_KEY_H
