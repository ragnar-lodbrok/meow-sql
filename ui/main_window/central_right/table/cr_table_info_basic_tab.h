#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_BASIC_TAB_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_BASIC_TAB_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class TableInfoForm;
}
}

namespace ui {
namespace main_window {
namespace central_right {
namespace table_info { // 5!

class BasicTab : public QWidget
{
    Q_OBJECT
public:
    explicit BasicTab(models::forms::TableInfoForm * form,
                      QWidget * parent = nullptr);

    void refreshData();

    void onBeforeEntityEditing();

private:

    void createWidgets();
    void fillDataFromForm();

    models::forms::TableInfoForm * _tableForm;

    QGridLayout * _mainGridLayout;

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _commentLabel;
    QPlainTextEdit * _commentEdit;
};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_BASIC_TAB_H
