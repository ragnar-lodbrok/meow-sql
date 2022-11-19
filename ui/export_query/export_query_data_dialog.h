#ifndef UI_EXPORT_QUERY_EXPORT_QUERY_DATA_DIALOG_H
#define UI_EXPORT_QUERY_EXPORT_QUERY_DATA_DIALOG_H

#include <QtWidgets>
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {

namespace models {
class BaseDataTableModel;
}

namespace export_query {

class OutputTargetWidget;
class OutputFormatWidget;
class RowSelectionWidget;
class OptionsWidget;

class Dialog : public QDialog
{
public:
    Dialog();
    void setData(models::BaseDataTableModel * model,
                 QItemSelectionModel * selection,
                 QTableView * tableView = nullptr);
private:
    void createWidgets();
    void showErrorMessage(const QString & message);

    Q_SLOT void onAccept();
    Q_SLOT void onFormatChanged();
    Q_SLOT void onFilenameChanged();
    Q_SLOT void onModeChanged();

    void validate();

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
