#ifndef UI_CENTRAL_RIGHT_VIEWTAB_H
#define UI_CENTRAL_RIGHT_VIEWTAB_H

#include <QtWidgets>

namespace meow {

namespace db {
    class ViewEntity;
}

namespace ui {
namespace main_window {
namespace central_right {

class ViewTab : public QWidget
{
public:
    explicit ViewTab(QWidget * parent = nullptr);
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_VIEWTAB_H
