#ifndef UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H
#define UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
class ExportQueryPresenter;
}

namespace export_query {

class RowSelectionWidget : public QWidget
{
public:
    explicit RowSelectionWidget(presenters::ExportQueryPresenter * presenter,
                                QWidget *parent = nullptr);
    void fillDataFromPresenter();
private:

    void createWidgets();

    Q_SLOT void onSelectionRadioButtonToggled(bool checked);

    presenters::ExportQueryPresenter * _presenter;

    QGroupBox * _groupBox;
    QRadioButton * _selectionRadioButton;
    QRadioButton * _completeRadioButton;
};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_OUTPUT_ROW_SELECTION_WIDGET_H
