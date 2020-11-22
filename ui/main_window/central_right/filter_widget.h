#ifndef UI_CENTRAL_RIGHT_FILTER_WIDGET_H
#define UI_CENTRAL_RIGHT_FILTER_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class FilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FilterWidget(QWidget *parent = nullptr);
    Q_SIGNAL void onFilterPatterChanged(const QString & pattern);

    void reset();

    void setFilterPattern(const QString & pattern);
    void setRowCount(int total, int matched);

private:
    void createWidgets();

    QLabel * _filterLabel;
    QLineEdit * _filterEdit;
    QLabel * _filteredStatsLabel;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_FILTER_WIDGET_H
