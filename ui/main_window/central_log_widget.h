#ifndef UI_CENTRAL_LOG_WIDGET_H
#define UI_CENTRAL_LOG_WIDGET_H

#include <QtWidgets>
#include "app/log.h"

namespace meow {
namespace ui {

namespace common {
class SQLLogEditor;
}


namespace main_window {

class CentralLogWidget : public QWidget, public Log::ISink
{
public:
    explicit CentralLogWidget(QWidget * parent = nullptr);

    void onLogMessage(const QString & msg) override;

private:

    Q_SLOT void onClearAction(bool checked);

    void createWidgets();

    QHBoxLayout * _mainLayout;
    ui::common::SQLLogEditor * _logEditor;
};

} // namespace main_window
} // namespace ui
} // namespace meow


#endif // UI_CENTRAL_LOG_WIDGET_H
