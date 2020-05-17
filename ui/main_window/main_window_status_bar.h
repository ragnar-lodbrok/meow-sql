#ifndef UI_MAIN_WINDOW_STATUS_BAR_H
#define UI_MAIN_WINDOW_STATUS_BAR_H

#include <QStatusBar>
#include <QPushButton>

namespace meow {
namespace ui {
namespace main_window {

class StatusBar : public QStatusBar
{

public:
    explicit StatusBar(QWidget *parent = nullptr);

private:
    QPushButton * _toggleLogButton;
};

} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_MAIN_WINDOW_STATUS_BAR_H
