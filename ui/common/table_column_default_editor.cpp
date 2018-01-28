#include "table_column_default_editor.h"

namespace meow {
namespace ui {

TableColumnDefaultEditor::TableColumnDefaultEditor(QWidget *parent)
    : QFrame(parent)
{

    setFrameStyle(QFrame::StyledPanel);
    setLineWidth(2);
    setAutoFillBackground(true);
    setWindowFlags(Qt::Popup);

    setMinimumSize(QSize(200, 300));
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
    _mainLayout->addWidget(_noRadioButton);

    _customRadioButton = new QRadioButton(tr("Custom") + ":");
    _mainLayout->addWidget(_customRadioButton);

    _customEdit = new QLineEdit;
    _mainLayout->addWidget(_customEdit);

}

} // namespace ui
} // namespace meow
