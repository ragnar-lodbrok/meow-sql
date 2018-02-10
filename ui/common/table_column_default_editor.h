#ifndef TABLE_COLUMN_DEFAULT_EDITOR_H
#define TABLE_COLUMN_DEFAULT_EDITOR_H

#include <QtWidgets>

namespace meow {

namespace db {
    enum class ColumnDefaultType;
}

namespace ui {

class TableColumnDefaultEditor : public QFrame
{
    Q_OBJECT
public:
    explicit TableColumnDefaultEditor(QWidget * parent = nullptr);

    void setDefaultValue(db::ColumnDefaultType type);
    void setDefaultText(const QString & text);

    void setAutoIncEditable(bool editable);
    void setCurrentTimeStampEditable(bool editable);
    void setOnUpdCurTsEditable(bool editable);

    db::ColumnDefaultType defaultValue() const;
    QString defaultText() const;

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

    bool _autoIncEditable;
    bool _curTsEditable;
    bool _onUpdCurTsEditable;

};

} // namespace ui
} // namespace meow

#endif // TABLE_COLUMN_DEFAULT_EDITOR_H
