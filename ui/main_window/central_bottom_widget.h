#ifndef UI_CENTRAL_BOTTOM_WIDGET_H
#define UI_CENTRAL_BOTTOM_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {

class CentralLogWidget;

class CentralBottomWidget : public QWidget
{
public:
    CentralBottomWidget(QWidget * parent = nullptr);
    ~CentralBottomWidget() override;

    virtual QSize sizeHint() const override {
        return QSize(0, 70);
    }

private:
    void createWidgets();

    QHBoxLayout * _mainLayout;
    CentralLogWidget * _logWidget;
};

} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_BOTTOM_WIDGET_H
