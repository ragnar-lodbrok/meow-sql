#include "main_window_status_bar.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace main_window {

StatusBar::StatusBar(QWidget *parent) : QStatusBar(parent)
{

    auto settings = meow::app()->settings()->geometrySettings();

    setSizeGripEnabled(false);
    setContentsMargins(0, 0, 0, 0);

    QAction * showFilterAction = meow::app()->actions()->showGlobalFilterPanel();

    _toggleShowFilterButton = new QPushButton(showFilterAction->text());
    _toggleShowFilterButton->setCheckable(true);
    _toggleShowFilterButton->setFlat(true);
    _toggleShowFilterButton->setChecked(settings->showFilterPanel());
    _toggleShowFilterButton->setToolTip(
                showFilterAction->shortcut().toString());

    _toggleLogButton = new QPushButton(tr("SQL Log"));
    _toggleLogButton->setCheckable(true);
    _toggleLogButton->setFlat(true);
    //_toggleLogButton->setMinimumHeight(20); // cut on win
    // TODO: style it (no radius etc)

    this->addPermanentWidget(_toggleShowFilterButton);
    this->addPermanentWidget(_toggleLogButton);

    _toggleShowFilterButton->setChecked(settings->showFilterPanel());
    _toggleLogButton->setChecked(settings->showSQLLog());

    connect(_toggleLogButton,
            &QAbstractButton::toggled,
            [=](bool checked) {
                settings->setShowSQLLog(checked);
            }
    );

    connect(_toggleShowFilterButton,
            &QAbstractButton::toggled,
            [=](bool checked) {
                settings->setShowFilterPanel(checked);
            }
    );

    connect(settings,
            &settings::Geometry::showFilterPanelChanged,
            [=](bool show){
                _toggleShowFilterButton->setChecked(show);
            }
    );
}

} // namespace main_window
} // namespace ui
} // namespace meow
