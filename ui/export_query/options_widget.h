#ifndef UI_EXPORT_QUERY_OPTIONS_WIDGET_H
#define UI_EXPORT_QUERY_OPTIONS_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace export_query {

class OptionsWidget : public QWidget
{
public:
    explicit OptionsWidget(QWidget *parent = nullptr);
private:

    void createWidgets();

    QGroupBox * _groupBox;

    QCheckBox * _includeColumnNamesCheckbox;
    QCheckBox * _includeAutoIncrementColumnCheckbox;
    QCheckBox * _includeSQLQueryCheckbox;
    QCheckBox * _removeLinebreaksCheckbox;

    QLabel * _separatorLabel;
    QLabel * _encloserLabel;
    QLabel * _terminatorLabel;
    QLabel * _nullValueLabel;

    QLineEdit * _separatorEdit;
    QLineEdit * _encloserEdit;
    QLineEdit * _terminatorEdit;
    QLineEdit * _nullValueEdit;
};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OPTIONS_WIDGET_H
