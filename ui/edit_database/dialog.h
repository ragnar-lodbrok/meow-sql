#ifndef UI_EDIT_DATABASE_DIALOG_H
#define UI_EDIT_DATABASE_DIALOG_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace edit_database {

class Dialog : public QDialog
{
public:
    Dialog();
private:
    void createWidgets();

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _collationLabel;
    QComboBox * _collationComboBox;

    QDialogButtonBox * _buttonBox;

};

} // namespace edit_database
} // namespace ui
} // namespace meow

#endif // UI_EDIT_DATABASE_DIALOG_H
