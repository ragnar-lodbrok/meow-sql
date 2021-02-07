#ifndef UI_CENTRAL_RIGHT_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_WIDGET_MODEL_H


// Main Window
//   Central Right Widget
//      Root Model

#include <QString>
#include <QIcon>
#include "db/entity/entity_holder.h"

namespace meow {
namespace models {
namespace ui {

// TODO: get rid of CentralRight or move into namespace ?

enum class CentralRightWidgetTabs {
    Host,
    Database,
    Entity, // Table, View, etc
    Data
};

class CentralRightWidgetModel
{
public:
    CentralRightWidgetModel();

    bool setCurrentEntity(meow::db::Entity * currentEntity);
    meow::db::Entity * currentEntity() const;

    QString titleForHostTab() const;
    QString titleForDatabaseTab() const;
    QString titleForEntityTab() const;
    QString titleForTableTab() const;
    QString titleForViewTab() const;
    QString titleForRoutineTab() const;
    QString titleForTriggerTab() const;
    QString titleForDataTab() const;
    QString titleForQueryTab() const;

    QIcon iconForRoutineTab() const;

    int indexForQueryTab() const;
    int indexForDataTab() const;

    bool connectionChanged() const;
    bool databaseChanged() const;
    bool hasDatabase() const;
    bool hasDataTab() const;
    bool hasEntityTab() const;
    bool hasQueryTab() const;

private:
    meow::db::EntityHolder _entityHolder;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_WIDGET_MODEL_H
