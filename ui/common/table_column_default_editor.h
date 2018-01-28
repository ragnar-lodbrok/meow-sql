#ifndef TABLE_COLUMN_DEFAULT_EDITOR_H
#define TABLE_COLUMN_DEFAULT_EDITOR_H

#include <QtWidgets>

namespace meow {
namespace ui {

class TableColumnDefaultEditor : public QFrame
{
    Q_OBJECT
public:
    explicit TableColumnDefaultEditor(QWidget * parent = nullptr);

private:

    void createWidgets();

    QVBoxLayout * _mainLayout;

    QRadioButton * _noRadioButton;

    QRadioButton * _customRadioButton;
    QLineEdit * _customEdit;

    QRadioButton * _nullRadioButton;

    QRadioButton * _currTSRadioButton;
    QCheckBox * _onUpdCurTSCheckBox;

    QRadioButton * _autoIncButton;

};

} // namespace ui
} // namespace meow

#endif // TABLE_COLUMN_DEFAULT_EDITOR_H
