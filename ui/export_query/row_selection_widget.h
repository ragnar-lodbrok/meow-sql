#ifndef UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace export_query {

class RowSelectionWidget : public QWidget
{
public:
    explicit RowSelectionWidget(QWidget *parent = nullptr);
private:

    void createWidgets();

    QGroupBox * _groupBox;
    QRadioButton * _selectionRadioButton;
    QRadioButton * _completeRadioButton;
};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H
