#ifndef MODELS_EDIT_DATABASE_FORM_H
#define MODELS_EDIT_DATABASE_FORM_H

#include <QStringList>

namespace meow {

namespace db {
   class SessionEntity;
   class DataBaseEntity;
}

namespace ui {
namespace presenters {

class EditDatabaseForm
{
public:
    EditDatabaseForm(meow::db::SessionEntity * session,
                     meow::db::DataBaseEntity * database = nullptr,
                     bool createMode = true);

    QString name() const;
    void setName(const QString & name);

    QString originalName() const;
    bool nameChanged() const {
        return name() != originalName();
    }

    const QStringList collations() const;

    QString collation() const;
    void setCollation(const QString & collation);

    bool isCreateMode() const { return _createMode; }
    bool dropOldDatabase() const { return _dropOldDatabase; }

    bool canSave() const;

    void save();

    bool databaseWithEnteredNameExists(bool refresh = false) const;

private:
    meow::db::SessionEntity * _session;
    meow::db::DataBaseEntity * _database;
    bool _createMode;
    bool _dropOldDatabase;

    QString _name;
    QString _collation;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MODELS_EDIT_DATABASE_FORM_H
