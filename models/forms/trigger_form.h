#ifndef MODELS_FORMS_TRIGGERFORM_H
#define MODELS_FORMS_TRIGGERFORM_H

#include <memory>
#include "db/entity/trigger_entity.h"

namespace meow {
namespace models {
namespace forms {

class TriggerForm
{
public:
    TriggerForm();

    void setTrigger(meow::db::TriggerEntity * trigger);

    QString name() const;

    QString statement() const;

    QString definer() const;
    QStringList allDefinerOptions(bool loadFromDB = false) const;
    bool supportsDefiner() const;

    QString tableName() const;
    QStringList allTableNameOptions() const;

    QString actionTime() const;
    QStringList allActionTimeOptions() const;

    QString event() const;
    QStringList allEventOptions() const;

    void setHasUnsavedChanges(bool modified);
    bool isEditingSupported() const;

private:
    std::unique_ptr<meow::db::TriggerEntity> _trigger;
    meow::db::TriggerEntity * _sourceTrigger;
};

} // namespace forms
} // namespace models
} // namespace meow

#endif // MODELS_FORMS_TRIGGERFORM_H
