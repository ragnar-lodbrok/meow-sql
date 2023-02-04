#ifndef UI_EXPORT_DATABASE_DIALOG_H
#define UI_EXPORT_DATABASE_DIALOG_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
    class ExportDatabaseForm;
}

namespace export_database {

class TopWidget;
class BottomWidget;

class Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Dialog(presenters::ExportDatabaseForm * form);

private:

    void createWidgets();
    void fillDataFromForm();

    Q_SLOT void onCancel();
    Q_SLOT void onExport();

    Q_SLOT void exportFinished(bool success);

    presenters::ExportDatabaseForm * _form;

    QVBoxLayout * _mainLayout;

    TopWidget * _topWidget;
    BottomWidget * _bottomWidget;
};

} // namespace export_database
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_DATABASE_DIALOG_H
