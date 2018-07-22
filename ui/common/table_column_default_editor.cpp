#include "table_column_default_editor.h"
#include "db/table_column.h"

namespace meow {
namespace ui {

TableColumnDefaultEditor::TableColumnDefaultEditor(QWidget *parent)
    : QFrame(parent),
      _autoIncEditable(false),
      _curTsEditable(false),
      _onUpdCurTsEditable(false)
{

    setFrameStyle(QFrame::StyledPanel);
    setLineWidth(2);
    setAutoFillBackground(true);
    setWindowFlags(Qt::Popup);

    setMinimumSize(QSize(280, 240));
    resize(minimumSize());

    createWidgets();
}

void TableColumnDefaultEditor::createWidgets()
{

    _mainLayout = new QVBoxLayout();
    _mainLayout->setAlignment(Qt::AlignTop);
    //_mainLayout->setSpacing(0);
    //_mainLayout->setMargin(0);
    setLayout(_mainLayout);

    _noRadioButton = new QRadioButton(tr("No default value"));
    connect(_noRadioButton, &QAbstractButton::toggled,
            [=](bool checked) {
                if (checked) {
                    _onUpdCurTSCheckBox->setEnabled(false);
                }
            });
    _mainLayout->addWidget(_noRadioButton);

    _customRadioButton = new QRadioButton(tr("Custom") + ":");
    connect(_customRadioButton, &QAbstractButton::toggled,
            [=](bool checked) {
                if (checked) {
                    _customEdit->setFocus();
                    _onUpdCurTSCheckBox->setEnabled(_onUpdCurTsEditable);
                }
            });
    _mainLayout->addWidget(_customRadioButton);

    _customEdit = new QLineEdit;
    _mainLayout->addWidget(_customEdit);

    _nullRadioButton = new QRadioButton("NULL");
    connect(_nullRadioButton, &QAbstractButton::toggled,
            [=](bool checked) {
                if (checked) {
                    _onUpdCurTSCheckBox->setEnabled(_onUpdCurTsEditable);
                }
            });
    _mainLayout->addWidget(_nullRadioButton);

    _currTSRadioButton = new QRadioButton("CURRENT_TIMESTAMP");
    connect(_currTSRadioButton, &QAbstractButton::toggled,
            [=](bool checked) {
                if (checked) {
                    _onUpdCurTSCheckBox->setEnabled(_onUpdCurTsEditable);
                }
            });
    _mainLayout->addWidget(_currTSRadioButton);

    _onUpdCurTSCheckBox = new QCheckBox("ON UPDATE CURRENT_TIMESTAMP");
    _mainLayout->addWidget(_onUpdCurTSCheckBox);

    _autoIncButton = new QRadioButton("AUTO_INCREMENT");
    connect(_autoIncButton, &QAbstractButton::toggled,
            [=](bool checked) {
                if (checked) {
                    _onUpdCurTSCheckBox->setEnabled(false);
                }
            });
    _mainLayout->addWidget(_autoIncButton);

}

void TableColumnDefaultEditor::setDefaultValue(db::ColumnDefaultType type)
{

    switch (type) {
    case db::ColumnDefaultType::None:
        _noRadioButton->setChecked(true);
        break;
    case db::ColumnDefaultType::Text:
        _customRadioButton->setChecked(true);
        _customEdit->setFocus();
        break;
    case db::ColumnDefaultType::TextUpdateTS:
        _customRadioButton->setChecked(true);
        _customEdit->setFocus();
        _onUpdCurTSCheckBox->setChecked(true);
        break;
    case db::ColumnDefaultType::Null:
        _nullRadioButton->setChecked(true);
        break;
    case db::ColumnDefaultType::NullUpdateTS:
        _nullRadioButton->setChecked(true);
        _onUpdCurTSCheckBox->setChecked(true);
        break;
    case db::ColumnDefaultType::CurTS:
        _currTSRadioButton->setChecked(true);
        break;
    case db::ColumnDefaultType::CurTSUpdateTS:
        _currTSRadioButton->setChecked(true);
        _onUpdCurTSCheckBox->setChecked(true);
        break;
    case db::ColumnDefaultType::AutoInc:
        _autoIncButton->setChecked(true);
        break;
    default:
        break;
    }
}

void TableColumnDefaultEditor::setDefaultText(const QString & text)
{
    _customEdit->setText(text);
}

void TableColumnDefaultEditor::setAutoIncEditable(bool editable)
{
    _autoIncEditable = editable;
    _autoIncButton->setEnabled(editable);
}

void TableColumnDefaultEditor::setCurrentTimeStampEditable(bool editable)
{
    _curTsEditable = editable;
    _currTSRadioButton->setEnabled(editable);
}

void TableColumnDefaultEditor::setOnUpdCurTsEditable(bool editable)
{
    _onUpdCurTsEditable = editable;
    _onUpdCurTSCheckBox->setEnabled(editable);
}

db::ColumnDefaultType TableColumnDefaultEditor::defaultValue() const
{
    if (_noRadioButton->isChecked()) {
        return db::ColumnDefaultType::None;
    }

    bool onUpdCurTS = _onUpdCurTSCheckBox->isChecked()
                   && _onUpdCurTSCheckBox->isEnabled();

    if (_customRadioButton->isChecked()) {
        if (onUpdCurTS) {
            return db::ColumnDefaultType::TextUpdateTS;
        } else {
            return db::ColumnDefaultType::Text;
        }
    }
    if (_nullRadioButton->isChecked() && _nullRadioButton->isEnabled()) {
        if (onUpdCurTS) {
            return db::ColumnDefaultType::NullUpdateTS;
        } else {
            return db::ColumnDefaultType::Null;
        }
    }
    if (_currTSRadioButton->isChecked() && _currTSRadioButton->isEnabled()) {
        if (onUpdCurTS) {
            return db::ColumnDefaultType::CurTSUpdateTS;
        } else {
            return db::ColumnDefaultType::CurTS;
        }
    }
    if (_autoIncButton->isChecked() && _autoIncButton->isEnabled()) {
        return db::ColumnDefaultType::AutoInc;
    }

    return db::ColumnDefaultType::None;
}

QString TableColumnDefaultEditor::defaultText() const
{
    db::ColumnDefaultType curType = defaultValue();
    if (curType == db::ColumnDefaultType::Text ||
            curType == db::ColumnDefaultType::TextUpdateTS) {
        return _customEdit->text();
    }
    return QString();
}

} // namespace ui
} // namespace meow
