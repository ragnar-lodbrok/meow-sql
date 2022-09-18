#ifndef UI_EXPORT_QUERY_EXPORT_QUERY_DATA_DIALOG_H
#define UI_EXPORT_QUERY_EXPORT_QUERY_DATA_DIALOG_H

#include <QtWidgets>
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {
namespace export_query {

class OutputTargetWidget;
class OutputFormatWidget;
class RowSelectionWidget;
class OptionsWidget;

class Dialog : public QDialog
{
public:
    Dialog();
private:
    void createWidgets();

    Q_SLOT void onAccept();

    ui::presenters::ExportQueryPresenter _presenter;

    OutputTargetWidget * _outputTargetWidget;
    OutputFormatWidget * _outputFormatWidget;
    RowSelectionWidget * _rowSelectionWidget;
    OptionsWidget * _optionsWidget;

    QDialogButtonBox * _buttonBox;
    QPushButton * _okButton;
    QPushButton * _cancelButton;
};

} // namespace export_query
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_QUERY_EXPORT_QUERY_DATA_DIALOG_H
