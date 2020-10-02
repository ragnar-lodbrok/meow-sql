#ifndef UI_EXPORT_DATABASE_TOP_WIDGET_H
#define UI_EXPORT_DATABASE_TOP_WIDGET_H

#include <QtWidgets>
#include "models/forms/export_database_form.h"

namespace meow {
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

    void setInputsEnabled(bool enabled);

    Q_SLOT void appendToResults(const QString & str);

private:
    void createWidgets();

    Q_SLOT void onOptionsCheckboxChanged(int state);
    Q_SLOT void onFormOptionsChanged();    
    Q_SLOT void onFilenameSelectionButtonClicked();

    models::forms::ExportDatabaseForm * _form;

    QGridLayout * _mainGridLayout;

    QLabel * _databaseToExportLabel;
    QComboBox * _databaseToExportComboBox;

    QLabel * _filenameLabel;
    QLineEdit * _filenameEdit;
    QPushButton * _filenameSelectionButton;

    QLabel * _optionsDatabasesLabel;
    QCheckBox * _databaseCreateCheckbox;
    QCheckBox * _databaseDropCheckbox;

    QLabel * _optionsTablesLabel;
    QCheckBox * _tablesCreateCheckbox;
    QCheckBox * _tablesDropCheckbox;

    QLabel * _optionsTriggersLabel;
    QCheckBox * _triggersCreateCheckbox;
    QCheckBox * _triggersDropCheckbox;

    QLabel * _optionsRoutinesEventsLabel;
    QCheckBox * _routinesCreateCheckbox;
    QCheckBox * _eventsCreateCheckbox;

    QCheckBox * _disableColumnStatisticsCheckbox;

    QPlainTextEdit * _results;

    QMap<QCheckBox *, models::forms::MySQLDumpOption> _checkboxOptions;
};

} // namespace export_database
} // namespace ui
} // namespace meow

#endif // UI_EXPORT_DATABASE_TOP_WIDGET_H
