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
        NoAction = 0,  // NO ACTION
        Restrict,  // RESTRICT
        Cascade,   // CASCADE
        SetNull,   // SET NULL
        SetDefault // SET DEFAULT
    };

    ForeignKey();

    QString name() const { return _name; }
    void setName(const QString & name) { _name = name; }

    QString referenceTable() const { return _referenceTable; }
    void setReferenceTable(const QString & refTable) {
        _referenceTable = refTable;
    }

    QStringList & columns() { return _columns; }
    QStringList & referenceColumns() { return _referenceColumns; }

    ReferenceOption onUpdate() const { return _onUpdate; }
    void setOnUpdate(ReferenceOption & opt) { _onUpdate = opt; }
    void setOnUpdate(const QString & opt);

    ReferenceOption onDelete() const { return _onDelete; }
    void setOnDelete(ReferenceOption & opt) { _onDelete = opt; }
    void setOnDelete(const QString & opt);

    operator QString() const;

private:
    QString _name;
    QString _referenceTable;
    ReferenceOption _onUpdate;
    ReferenceOption _onDelete;
    QStringList _columns;
    QStringList _referenceColumns;

};

ForeignKey::ReferenceOption strToReferenceOption(const QString & str);
QString referenceOptionToStr(ForeignKey::ReferenceOption opt);

} // namespace db
} // namespace meow

#endif // FOREIGN_KEY_H
