#include "main_window_status_bar.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{

    setSizeGripEnabled(false);
    setContentsMargins(0 , 0, 0, 0);

    _toggleLogButton = new QPushButton(tr("SQL Log"));
    _toggleLogButton->setCheckable(true);
    _toggleLogButton->setFlat(true);
    _toggleLogButton->setMinimumHeight(20);
    // TODO: style it (no radius etc)

    this->addPermanentWidget(_toggleLogButton);

    auto settings = meow::app()->settings()->geometrySettings();
    _toggleLogButton->setChecked(settings->showSQLLog());

    connect(_toggleLogButton,
            &QAbstractButton::toggled,
            [=](bool checked) {
                auto settings = meow::app()->settings()->geometrySettings();
                settings->setShowSQLLog(checked);
            }
    );
}

} // namespace main_window
} // namespace ui
} // namespace meow
