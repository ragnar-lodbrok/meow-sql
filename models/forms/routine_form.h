#ifndef MODELS_FORMS_ROUTINEFORM_H
#define MODELS_FORMS_ROUTINEFORM_H

#include <QObject>
#include <QString>
#include <QMap>
#include <memory>
#include "db/entity/routine_entity.h"
#include "models/db/routine_parameters_model.h"

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
    void setComment(const QString & comment);

    meow::db::Entity::Type type() const {
        return _routine ? _routine->type() : meow::db::Entity::Type::Procedure;
    }
    QMap<meow::db::Entity::Type, QString> typeNames() const;
    void setType(const meow::db::Entity::Type type);

    QString returnType() const {
        return _routine ? _routine->structure()->returnType : QString();
    }
    QStringList returnTypes() const;
    bool supportsReturnType() const;
    void setReturnType(const QString & type);

    QString dataAccess() const;
    QStringList dataAccessList() const;
    void setDataAccess(const QString & dataAccess);

    QString security() const {
        return _routine ? _routine->structure()->sqlSecurity : QString();
    }
    QStringList securityOptions() const;
    bool supportsSecurity() const;
    void setSecurity(const QString & security);

    bool isDeterministic() const {
        return _routine ? _routine->structure()->deterministic : false;
    }
    void setDeterministic(bool on);

    QString body() const {
        return _routine ? _routine->structure()->body : QString();
    }
    void setBody(const QString & body);

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

    bool isEditingSupported() const;

    models::db::RoutineParametersModel * parametersModel() {
        return &_paramsModel;
    }

    meow::db::RoutineEntity * sourceRoutine() {
        return _sourceRoutine;
    }

    meow::db::RoutineEntity * editableRoutine() {
        return _routine.get();
    }

private:

    Q_SLOT void onParamsModified() {
        setHasUnsavedChanges(true);
    }

    std::unique_ptr<meow::db::RoutineEntity> _routine; // copy of source to edit
    meow::db::RoutineEntity * _sourceRoutine;

    bool _hasUnsavedChanges; // TODO: do we really need to store it?

    models::db::RoutineParametersModel _paramsModel;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_ROUTINEFORM_H
