#include "settings_geometry.h"
#include <QSettings>

namespace meow {
namespace settings {

static const char SHOW_LOG_SETTINGS_KEY[]
    = "ui/main_windows/status_bar/show_log";

Geometry::Geometry()
{
    load();
}

int Geometry::tableViewDefaultRowHeight() const
{
    return 24;
}

bool Geometry::showSQLLog() const
{
    return _showSQLLog;
}

void Geometry::setShowSQLLog(bool show)
{
    if (_showSQLLog != show) {
        _showSQLLog = show;
        QSettings settings;
        settings.setValue(SHOW_LOG_SETTINGS_KEY, show);
        emit showSQLLogChanged(show);
    }
}

void Geometry::load()
{
    QSettings settings;
    _showSQLLog = settings.value(SHOW_LOG_SETTINGS_KEY, false).toBool();

}

} // namespace meow
} // namespace settings
