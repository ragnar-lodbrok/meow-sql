#ifndef UI_CENTRAL_RIGHT_ADD_QUERY_TAB_H
#define UI_CENTRAL_RIGHT_ADD_QUERY_TAB_H

#include <QtWidgets>
#include "ui/main_window/central_right/base_root_tab.h"

namespace meow {

namespace ui {
namespace main_window {
namespace central_right {

class AddQueryTab : public BaseRootTab
{
    Q_OBJECT
public:
    explicit AddQueryTab(QWidget *parent = nullptr);

private:
    void createWidgets();
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_ADD_QUERY_TAB_H
