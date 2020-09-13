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

namespace db {
    class VariablesTableModel;
}

namespace ui {

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

    void setVariablesModel(models::db::VariablesTableModel * model) {
        _variablesModel = model;
    }

    bool showVariablesTab() const;

private:
    meow::db::SessionEntity * _curEntity;
    models::db::VariablesTableModel * _variablesModel;
};

} // namespace ui
} // namespace models
} // namespace meow

#endif // UI_CENTRAL_RIGHT_HOST_WIDGET_MODEL_H
