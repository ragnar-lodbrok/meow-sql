#ifndef UI_CENTRALLEFTWIDGET_H
#define UI_CENTRALLEFTWIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {

class CentralLeftWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CentralLeftWidget(QWidget *parent = 0);
private:
    QVBoxLayout * _mainLayout;
    QTreeView * _dbTree;
    void createMainLayout();

};


} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRALLEFTWIDGET_H
