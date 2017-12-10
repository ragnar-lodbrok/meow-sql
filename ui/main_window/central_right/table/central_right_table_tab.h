#ifndef UI_CENTRAL_RIGHT_TABLETAB_H
#define UI_CENTRAL_RIGHT_TABLETAB_H

#include <QtWidgets>

namespace meow {

namespace db {
    class TableEntity;
}

namespace ui {
namespace main_window {
namespace central_right {

class TableInfo;
class TableColumns;

class TableTab : public QWidget
{
    Q_OBJECT
public:

    explicit TableTab(QWidget * parent = 0);

    void setTable(db::TableEntity * table);
private:

    void createWidgets();

    QSplitter * _mainSplitter;

    TableInfo * _tableInfo;
    TableColumns * _columns;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLETAB_H
