#ifndef UI_CR_HOST_DATABASES_TAB_H
#define UI_CR_HOST_DATABASES_TAB_H

#include <QtWidgets>
#include "models/db/databases_table_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class HostDatabasesTab : public QWidget
{
    Q_OBJECT
public:
    explicit HostDatabasesTab(QWidget *parent = 0);
    void setSession(meow::db::SessionEntity * session);
private:

    void createDatabasesTable();

    models::db::DatabasesTableModel _model;

    QVBoxLayout * _mainLayout;
    QTableView  * _databasesTable;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CR_HOST_DATABASES_TAB_H
