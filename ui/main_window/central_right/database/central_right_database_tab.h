#ifndef UI_CENTRALRIGHT_DATABASETAB_H
#define UI_CENTRALRIGHT_DATABASETAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"
#include "ui/models/database_entities_table_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class DatabaseTab : public BaseRootTab
{
    Q_OBJECT
public:
    explicit DatabaseTab(QWidget *parent = 0);

    void setDataBase(meow::db::DataBaseEntity * database);

private:

    void createEntitiesTable();

    models::DatabaseEntitiesTableModel _model;
    QSortFilterProxyModel _proxyEntitiesModel;

    QVBoxLayout * _mainLayout;
    QTableView  * _entitiesTable;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRALRIGHT_DATABASETAB_H
