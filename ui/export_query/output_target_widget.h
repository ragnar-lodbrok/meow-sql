#ifndef UI_EXPORT_QUERY_OUTPUT_TARGET_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_TARGET_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
class ExportQueryPresenter;
}

namespace export_query {

class OutputTargetWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OutputTargetWidget(presenters::ExportQueryPresenter * presenter,
                                QWidget *parent = nullptr);
private:

    void createWidgets();
    void fillDataFromPresenter();
    void validateControls();

    Q_SLOT void onModeRadioButtonToggled(bool checked);
    Q_SLOT void onEncodingComboboxTextChanged(const QString & text);

    presenters::ExportQueryPresenter * _presenter;

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
