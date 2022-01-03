#ifndef UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H
#define UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H

#include <QString>
#include "db/entity/session_entity.h"

// Main Window
//   Central Right Widget
//     Host Tab
//       Root Model

namespace meow {
namespace ui {

namespace models {
    class VariablesTableModel;
}

namespace presenters {

class CentralRightHostWidgetModel
{
public:
    CentralRightHostWidgetModel();
    bool setCurrentEntity(meow::db::SessionEntity * curEntity);
    QString titleForDatabasesTab() const;
    QString titleForVariablesTab() const;

    meow::db::SessionEntity * currentSession() const {
        return _curEntity;
    }

    void setVariablesModel(models::VariablesTableModel * model) {
        _variablesModel = model;
    }

    bool showVariablesTab() const;

private:
    meow::db::SessionEntity * _curEntity;
    models::VariablesTableModel * _variablesModel;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H
