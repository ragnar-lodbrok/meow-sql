#ifndef UI_CR_HOST_DATABASES_TAB_H
#define UI_CR_HOST_DATABASES_TAB_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class HostDatabasesTab : public QWidget
{
    Q_OBJECT
public:
    explicit HostDatabasesTab(QWidget *parent = 0);
private:

    void createDatabasesTable();

    QVBoxLayout * _mainLayout;
    QTableView  * _databasesTable;

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CR_HOST_DATABASES_TAB_H
