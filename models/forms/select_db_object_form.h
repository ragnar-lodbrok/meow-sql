#ifndef MODELS_FORMS_SELECT_DB_OBJECT_FORM_H
#define MODELS_FORMS_SELECT_DB_OBJECT_FORM_H

#include <QObject>
#include "db/entity/session_entity.h"

namespace meow {
namespace models {
namespace forms {

class SelectDBObjectForm : public QObject
{
    Q_OBJECT
public:
    struct Object {
        Object(
               const meow::db::Entity::Type _type = meow::db::Entity::Type::None,
               const QString & _databaseName = QString(),
               const QString & _entityName = QString(),
               const QString & _fieldName = QString()
               )
            : type(_type)
            , databaseName(_databaseName)
            , entityName(_entityName)
            , fieldName(_fieldName)
             { }
        meow::db::Entity::Type type;
        QString databaseName;
        QString entityName;
        QString fieldName;
    };

    SelectDBObjectForm(meow::db::SessionEntity * session);

    void setTreeSelection(const Object & object);
    void setDatabaseWildcard(const QString & databaseName);

    const Object & currentSelection() const {
        return _currentSelection;
    }

    bool hasValidSelection() const {
        return !_currentSelection.databaseName.isEmpty();
    }

    Q_SIGNAL void databasenameChanged(const QString & name);

private:
    Object _currentSelection;
    meow::db::SessionEntity * _session;
};

} // namespace forms
} // namespace models
} // namespace meow


#endif // MODELS_FORMS_SELECT_DB_OBJECT_FORM_H
