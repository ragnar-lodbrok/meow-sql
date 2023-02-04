#ifndef UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
class ExportQueryPresenter;
}

namespace export_query {

class OutputFormatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OutputFormatWidget(presenters::ExportQueryPresenter * presenter,
                                QWidget *parent = nullptr);
private:

    void createWidgets();
    void fillDataFromPresenter();

    Q_SLOT void onFormatComboboxIndexChanged(int index);
    Q_SLOT void onPresenterFormatChanged();

    presenters::ExportQueryPresenter * _presenter;

    QGroupBox * _groupBox;
    QComboBox * _formatCombobox;

};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OUTPUT_FORMAT_WIDGET_H
