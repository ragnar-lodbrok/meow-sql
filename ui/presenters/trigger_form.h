#ifndef MODELS_FORMS_TRIGGERFORM_H
#define MODELS_FORMS_TRIGGERFORM_H

#include <memory>
#include "db/entity/trigger_entity.h"

namespace meow {
namespace ui {
namespace presenters {

class TriggerForm : public QObject
{
    Q_OBJECT
public:
    TriggerForm();

    void setTrigger(const meow::db::TriggerEntityPtr & trigger);

    QString name() const;
    void setName(const QString & name);

    QString statement() const;
    void setStatement(const QString & statement);

    QString definer() const;
    void setDefiner(const QString & definer);
    QStringList allDefinerOptions(bool loadFromDB = false) const;
    bool supportsDefiner() const;

    QString tableName() const;
    void setTableName(const QString & tableName);
    QStringList allTableNameOptions() const;

    QString actionTime() const;
    void setActionTime(const QString & actionTime);
    QStringList allActionTimeOptions() const;

    QString eventName() const;
    void setEventName(const QString & eventName);
    QStringList allEventOptions() const;

    void save();

    bool hasUnsavedChanges() const { return _hasUnsavedChanges; }
    void setHasUnsavedChanges(bool modified);
    Q_SIGNAL void unsavedChanged(bool hasUnsavedChanges);

    bool isEditingSupported() const;

    meow::db::TriggerEntityPtr sourceTrigger() {
        return _sourceTrigger;
    }

private:

    void setDefaultValuesForNew();

    meow::db::TriggerEntityPtr _trigger;
    meow::db::TriggerEntityPtr _sourceTrigger;

    bool _hasUnsavedChanges;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MODELS_FORMS_TRIGGERFORM_H
