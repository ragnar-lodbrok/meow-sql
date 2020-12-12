#ifndef MODELS_FORMS_ROUTINEFORM_H
#define MODELS_FORMS_ROUTINEFORM_H

#include <QObject>
#include <QString>
#include <QMap>
#include <memory>
#include "db/entity/routine_entity.h"

namespace meow {
namespace models {
namespace forms {


class RoutineForm : public QObject
{
    Q_OBJECT
public:
    RoutineForm();

    void setRoutine(meow::db::RoutineEntity * routine);

    QString name() const {
        return _routine ? _routine->name() : QString();
    }
    void setName(const QString & name);

    QString definer() const;
    QStringList allDefinerOptions(bool loadFromDB = false) const;
    void setDefiner(const QString & definer);
    bool supportsDefiner() const;

    QString comment() const {
        return _routine ? _routine->structure()->comment : QString();
    }

    meow::db::Entity::Type type() const {
        return _routine ? _routine->type() : meow::db::Entity::Type::Procedure;
    }
    QMap<meow::db::Entity::Type, QString> typeNames() const;

    QString returnType() const {
        return _routine ? _routine->structure()->returnType : QString();
    }
    QStringList returnTypes() const;
    bool supportsReturnType() const;

    QString dataAccess() const;
    QStringList dataAccessList() const;

    QString security() const {
        return _routine ? _routine->structure()->sqlSecurity : QString();
    }
    QStringList securityOptions() const;
    bool supportsSecurity() const;

    bool isDeterministic() const {
        return _routine ? _routine->structure()->deterministic : false;
    }

    QString body() const {
        return _routine ? _routine->structure()->body : QString();
    }

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

    bool isEditingSupported() const;

private:
    std::unique_ptr<meow::db::RoutineEntity> _routine; // copy of source to edit
    meow::db::RoutineEntity * _sourceRoutine;

    bool _hasUnsavedChanges; // TODO: do we really need to store it?
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_ROUTINEFORM_H
