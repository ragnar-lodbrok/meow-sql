#ifndef UI_CR_HOST_VARIABLES_TAB_H
#define UI_CR_HOST_VARIABLES_TAB_H

#include <QtWidgets>
#include "ui/models/variables_table_model.h"

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

    models::VariablesTableModel * model() {
        return &_model;
    }
private:

    void createVariablesTable();

    Q_SLOT void onTableContextMenu(const QPoint &pos);
    Q_SLOT void onEditError(const QString & message);

    models::VariablesTableModel _model;

    QVBoxLayout * _mainLayout;
    QTableView  * _variablesTable;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CR_HOST_VARIABLES_TAB_H
