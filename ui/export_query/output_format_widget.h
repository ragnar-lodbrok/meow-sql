#ifndef UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace export_query {

class OutputFormatWidget : public QWidget
{
public:
    explicit OutputFormatWidget(QWidget *parent = nullptr);
private:

    void createWidgets();

    QGroupBox * _groupBox;
    QComboBox * _formatCombobox;

};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H
