#ifndef UI_EXPORT_DATABASE_DIALOG_H
#define UI_EXPORT_DATABASE_DIALOG_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class ExportDatabaseForm;
}
}

namespace ui {
namespace export_database {

class TopWidget;
class BottomWidget;

class Dialog : public QDialog
{
public:
    explicit Dialog(models::forms::ExportDatabaseForm * form);

private:

    void createWidgets();
    void fillDataFromForm();

    Q_SLOT void onCancel();
    Q_SLOT void onExport();

    Q_SLOT void exportFinished(bool success);

    models::forms::ExportDatabaseForm * _form;

    QVBoxLayout * _mainLayout;

    TopWidget * _topWidget;
    BottomWidget * _bottomWidget;
};

} // namespace export_database
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_DATABASE_DIALOG_H
