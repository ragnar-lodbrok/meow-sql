#ifndef UI_MAINWINDOW_CENTRALWIDGET_H
#define UI_MAINWINDOW_CENTRALWIDGET_H

#include <QtWidgets>
#include "central_left_widget.h"
#include "central_right_widget.h"

namespace meow {
namespace ui {
namespace main_window {

class CentralWidget : public QWidget
{
public:
    explicit CentralWidget(QWidget * parent = nullptr);
private:

    void createMainLayout();

    QHBoxLayout * _mainLayout;
    QSplitter * _mainHorizontalSplitter;

    CentralLeftWidget * _mainLeftWidget;
    CentralRightWidget * _mainRightWidget;
};

} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_MAINWINDOW_CENTRALWIDGET_H
