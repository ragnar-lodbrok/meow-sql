#ifndef UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H

#include <QString>
#include "db/entity/session_entity.h"

// Main Window
//   Central Right Widget
//     Host Tab
//       Root Model

namespace meow {
namespace models {
namespace ui {

class CentralRightHostWidgetModel
{
public:
    CentralRightHostWidgetModel();
    bool setCurrentEntity(meow::db::SessionEntity * curEntity);
    QString titleForDatabasesTab() const;
private:
    meow::db::SessionEntity * _curEntity;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H
