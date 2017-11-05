#ifndef UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H
#define UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

// Intent: Tools widget for table columns widget
class TableColumnsTools : public QWidget
{
public:
    TableColumnsTools(QWidget * parent);
private:
    void createWidgets();

    QLabel * _columnsLabel;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_COLUMNS_TOOLS_H
