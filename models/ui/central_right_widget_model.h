#ifndef UI_CENTRAL_RIGHT_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_WIDGET_MODEL_H

#include "db/entity/entity.h"
#include <QString>

// Main Window
//   Central Right Widget
//      Root Model

namespace meow {
namespace models {
namespace ui {

enum CentralRightWidgetTabs {
    Host,
    Database
};

class CentralRightWidgetModel
{
public:
    CentralRightWidgetModel();
    bool setCurrentEntity(meow::db::Entity * currentEntity);
    QString titleForHostTab() const;
    bool connectionChanged() const;
private:
    meow::db::Entity * _prevEntity;
    meow::db::Entity * _currentEntity;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_WIDGET_MODEL_H
