#ifndef UI_EXPORT_DATABASE_TOP_WIDGET_H
#define UI_EXPORT_DATABASE_TOP_WIDGET_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class ExportDatabaseForm;
}
}

namespace ui {
namespace export_database {

class TopWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopWidget(models::forms::ExportDatabaseForm * form,
                       QWidget * parent = nullptr);

    void fillDataFromForm();

    void clearResults();
    Q_SLOT void appendToResults(const QString & str);

private:
    void createWidgets();

    models::forms::ExportDatabaseForm * _form;

    QGridLayout * _mainGridLayout;

    QLabel * _databaseToExportLabel;
    QComboBox * _databaseToExportComboBox;

    QLabel * _filenameLabel;
    QLineEdit * _filenameEdit;

    QPlainTextEdit * _results;
};

} // namespace export_database
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_DATABASE_TOP_WIDGET_H
