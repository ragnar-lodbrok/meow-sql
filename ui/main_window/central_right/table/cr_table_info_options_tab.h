#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_OPTIONS_TAB_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_OPTIONS_TAB_H

#include <QtWidgets>

namespace meow {
namespace ui {

namespace presenters {
    class TableInfoForm;
}

namespace main_window {
namespace central_right {
namespace table_info {

class OptionsTab : public QWidget
{
    Q_OBJECT
public:
    explicit OptionsTab(presenters::TableInfoForm * form,
                        QWidget *parent = nullptr);
    void refreshData();

private:

    void createWidgets();
    void fillDataFromForm();

    presenters::TableInfoForm * _tableForm;

    QGridLayout * _mainGridLayout;

    QLabel * _autoIncLabel;
    QLineEdit * _autoIncEdit;

    QLabel * _defaultCollationLabel;
    QComboBox * _defaultCollationComboBox;

    QLabel * _avgRowLenLabel;
    QLineEdit * _avgRowLenEdit;

    QLabel * _engineLabel;
    QComboBox * _engineComboBox;

    QLabel * _maxRowCountLabel;
    QLineEdit * _maxRowCountEdit;

    //QLabel * _unionTablesLabel;
    //QPlainTextEdit * _unionTablesEdit;

    QCheckBox * _checksumForRowsCheckBox;

    QLabel * _rowFormatLabel;
    QComboBox * _rowFormatComboBox;

    //QLabel * _insertMethodLabel;
    //QComboBox * _insertMethodComboBox;

};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_OPTIONS_TAB_H
