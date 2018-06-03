#ifndef FOREIGN_KEY_H
#define FOREIGN_KEY_H

#include <QString>
#include <QStringList>

namespace meow {
namespace db {

class TableStructure;
class TableColumn;
class TableEntity;

// Represents foreign key
class ForeignKey
{
public:

    enum class ReferenceOption {
        NoAction = 0,  // NO ACTION
        Restrict,  // RESTRICT
        Cascade,   // CASCADE
        SetNull,   // SET NULL
        SetDefault, // SET DEFAULT

        Count
    };

    explicit ForeignKey(TableEntity * table);

    void setTable(TableEntity * table) { _table = table; }
    TableEntity * table() const { return _table; }

    QString name() const { return _name; }
    void setName(const QString & name) { _name = name; }

    // TODO: use TableEntity instead ?
    QString referenceTable() const { return _referenceTable; }
    void setReferenceTable(const QString & refTable) {
        _referenceTable = refTable;
    }

    QStringList columnNames() const;
    void setColumns(const QStringList & columnNames);
    QList<TableColumn *> & columns() { return _columns; }

    QStringList & referenceColumns() { return _referenceColumns; }

    ReferenceOption onUpdate() const { return _onUpdate; }
    void setOnUpdate(ReferenceOption & opt) { _onUpdate = opt; }
    void setOnUpdate(const QString & opt);

    ReferenceOption onDelete() const { return _onDelete; }
    void setOnDelete(ReferenceOption & opt) { _onDelete = opt; }
    void setOnDelete(const QString & opt);

    unsigned id() const { return _id; }
    void setId(unsigned id) { _id = id; }

    bool isCustomName() const { return _isCustomName; }
    void setIsCustomName(bool val) { _isCustomName = val; }

    ForeignKey * deepCopy(TableStructure * structure);

    bool removeColumnByName(const QString & columnName);

    operator QString() const;

private:
    TableEntity * _table;
    unsigned _id;
    QString _name;
    bool _isCustomName;
    QString _referenceTable;
    ReferenceOption _onUpdate;
    ReferenceOption _onDelete;
    QList<TableColumn *> _columns; // no ownership
    QStringList _referenceColumns;

};

ForeignKey::ReferenceOption strToReferenceOption(const QString & str);
QString referenceOptionToStr(ForeignKey::ReferenceOption opt);
QStringList referenceOptionNames();

} // namespace db
} // namespace meow

#endif // FOREIGN_KEY_H
