#include "table_cell_line_edit.h"
#include "ui/common/text_editor_popup.h"

namespace meow {
namespace ui {

TableCellLineEdit::TableCellLineEdit(QWidget * parent)
    :QWidget(parent)
{    
     auto layout = new QHBoxLayout();

     layout->setSpacing(0);
     layout->setMargin(0);

     _lineEdit = new QLineEdit();
     _lineEdit->setFrame(0);

     _openPopupEditorButton = new QPushButton(tr("..."));
     _openPopupEditorButton->setMaximumWidth(
        _openPopupEditorButton->fontMetrics().boundingRect(
            _openPopupEditorButton->text())
            .width() + 10
     );
     _openPopupEditorButton->setToolTip(tr("Edit text in popup editor ..."));
     connect(_openPopupEditorButton,
             &QAbstractButton::clicked,
             this,
             &TableCellLineEdit::onOpenPopupEditor
     );


     layout->addWidget(_lineEdit, 10);
     layout->addWidget(_openPopupEditorButton, 1);

     setLayout(layout);
}

void TableCellLineEdit::onOpenPopupEditor()
{
    ui::TextEditorPopup editor;
    editor.exec();
}

} // namespace ui
} // namespace meow
