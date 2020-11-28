#ifndef MEOW_SETTINGS_GEOMETRY_H
#define MEOW_SETTINGS_GEOMETRY_H

#include <QObject>

namespace meow {
namespace settings {

// Settings for sizes, rects etc
class Geometry : public QObject
{
    Q_OBJECT

public:
    Geometry();
    int tableViewDefaultRowHeight() const;

    bool showSQLLog() const;
    void setShowSQLLog(bool show);
    Q_SIGNAL void showSQLLogChanged(bool show);

    bool showFilterPanel() const;
    void setShowFilterPanel(bool show);
    Q_SIGNAL void showFilterPanelChanged(bool show);

private:

    void load();
    bool _showSQLLog;
    bool _showFilterPanel;
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_GEOMETRY_H
