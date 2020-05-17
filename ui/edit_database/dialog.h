#ifndef UI_EDIT_DATABASE_DIALOG_H
#define UI_EDIT_DATABASE_DIALOG_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class EditDatabaseForm;
}
}

namespace ui {
namespace edit_database {

class Dialog : public QDialog
{
public:
    explicit Dialog(models::forms::EditDatabaseForm * form);
private:
    void createWidgets();
    void fillDataFromForm();
    void validate();

    Q_SLOT void onAccept();

    models::forms::EditDatabaseForm * _form;

    QLabel * _nameLabel;
    QLineEdit * _nameEdit;

    QLabel * _collationLabel;
    QComboBox * _collationComboBox;

    QDialogButtonBox * _buttonBox;
    QPushButton * _okButton;
    QPushButton * _cancelButton;

};

} // namespace edit_database
} // namespace ui
} // namespace meow

#endif // UI_EDIT_DATABASE_DIALOG_H
