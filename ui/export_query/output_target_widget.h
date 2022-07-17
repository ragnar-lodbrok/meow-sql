#ifndef UI_EXPORT_QUERY_OUTPUT_TARGET_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_TARGET_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace export_query {

class OutputTargetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OutputTargetWidget(QWidget *parent = nullptr);
private:

    void createWidgets();

    QGroupBox * _groupBox;

    QRadioButton * _clipboardRadioButton;
    QRadioButton * _fileRadioButton;

    QLineEdit * _filenameEdit;
    QPushButton * _filenameSelectionButton;

    QLabel * _encodingLabel;
    QComboBox * _encodingCombobox;
};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OUTPUT_TARGET_WIDGET_H
