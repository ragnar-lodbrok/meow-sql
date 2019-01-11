#ifndef MODELS_EDIT_DATABASE_FORM_H
#define MODELS_EDIT_DATABASE_FORM_H

#include <QStringList>

namespace meow {

namespace db {
   class Connection;
}

namespace models {
namespace forms {

class EditDatabaseForm
{
public:
    EditDatabaseForm(meow::db::Connection * connection);

    QString name() const;
    void setName(const QString & name);

    const QStringList collations() const;

    QString collation() const;
    void setCollation(const QString & collation);

    bool canSave() const;

    void save();

private:
    meow::db::Connection * _connection;

    QString _name;
    QString _collation;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_EDIT_DATABASE_FORM_H
