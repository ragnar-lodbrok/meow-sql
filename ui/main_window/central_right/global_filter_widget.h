#ifndef UI_CENTRAL_RIGHT_GLOBAL_FILTER_WIDGET_H
#define UI_CENTRAL_RIGHT_GLOBAL_FILTER_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

class GlobalFilterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GlobalFilterWidget(QWidget *parent = nullptr);
    Q_SIGNAL void onFilterPatternChanged(const QString & pattern,
                                        bool regexp = false);

    void reset();

    void setFilterPattern(const QString & pattern, bool regexp = false);
    void setRowCount(int total, int matched);

    void setFocus();

private:
    void createWidgets();

    Q_SLOT void onFilterEditTextChanged(const QString & text);
    Q_SLOT void onUseRegexpCheckboxChanged(int state);


    QLabel * _filterLabel;
    QLineEdit * _filterEdit;
    QLabel * _filteredStatsLabel;
    QCheckBox * _useRegexpCheckbox;
};

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_GLOBAL_FILTER_WIDGET_H
