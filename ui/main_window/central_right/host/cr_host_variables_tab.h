#ifndef UI_CR_HOST_VARIABLES_TAB_H
#define UI_CR_HOST_VARIABLES_TAB_H

#include <QtWidgets>
#include "models/db/variables_table_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class HostVariablesTab : public QWidget
{
    Q_OBJECT
public:
    explicit HostVariablesTab(QWidget *parent = 0);
    void setSession(meow::db::SessionEntity * session);

    models::db::VariablesTableModel * model() {
        return &_model;
    }
private:

    void createVariablesTable();

    models::db::VariablesTableModel _model;

    QVBoxLayout * _mainLayout;
    QTableView  * _variablesTable;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CR_HOST_VARIABLES_TAB_H
