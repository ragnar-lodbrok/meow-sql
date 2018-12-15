#ifndef UI_CENTRAL_LOG_WIDGET_H
#define UI_CENTRAL_LOG_WIDGET_H

#include <QtWidgets>
#include "app/log.h"

namespace meow {
namespace ui {

namespace common {
class SQLEditor;
}


namespace main_window {

class CentralLogWidget : public QWidget, public Log::ISink
{
public:
    explicit CentralLogWidget(QWidget * parent = nullptr);

    void onLogMessage(const QString & msg) override;

private:

    void createWidgets();

    void appendMessage(const QString & message);

    QHBoxLayout * _mainLayout;
    ui::common::SQLEditor * _logEditor;
};

} // namespace main_window
} // namespace ui
} // namespace meow


#endif // UI_CENTRAL_LOG_WIDGET_H
