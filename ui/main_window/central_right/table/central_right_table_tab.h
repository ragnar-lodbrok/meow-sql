#ifndef UI_CENTRAL_RIGHT_TABLETAB_H
#define UI_CENTRAL_RIGHT_TABLETAB_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class TableTab : public QWidget
{
    Q_OBJECT
public:

    enum Tabs {
        Basic,
        Count
    };

    explicit TableTab(QWidget *parent = 0);

};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLETAB_H
