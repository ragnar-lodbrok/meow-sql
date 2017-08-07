#ifndef UI_CENTRAL_RIGHT_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_WIDGET_MODEL_H

#include <QString>
#include "db/entity/entity_holder.h"

// Main Window
//   Central Right Widget
//      Root Model

namespace meow {
namespace models {
namespace ui {

enum CentralRightWidgetTabs {
    Host,
    Database,
    Entity,
    Data
};

class CentralRightWidgetModel
{
public:
    CentralRightWidgetModel();
    bool setCurrentEntity(meow::db::Entity * currentEntity);

    QString titleForHostTab() const;
    QString titleForDatabaseTab() const;
    QString titleForTableTab() const;
    QString titleForDataTab() const;

    int indexForQueryTab() const;

    bool connectionChanged() const;
    bool databaseChanged() const;
    bool hasDatabase() const;
    bool hasDataTab() const;
    bool hasEntityTab() const;

private:
    meow::db::EntityHolder _entityHolder;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_WIDGET_MODEL_H
