#ifndef UI_USER_MANAGER_WINDOW_H
#define UI_USER_MANAGER_WINDOW_H

#include <QtWidgets>

namespace meow {

namespace db {
    class SessionEntity;
}

namespace ui {

namespace main_window {
    class Window;
}

namespace user_manager {

class LeftWidget;
class RightWidget;

class Window : public QDialog
{
public:
    explicit Window(main_window::Window * mainWindow,
                    db::SessionEntity * session);
    ~Window() override;

private:

    void createWidgets();

    main_window::Window * _mainWindow;
    db::SessionEntity * _session;

    QSplitter * _splitter;
    LeftWidget * _leftWidget;
    RightWidget * _rightWidget;
};

} // namespace user_manager
} // namespace ui
} // namespace meow

#endif // UI_USER_MANAGER_WINDOW_H
