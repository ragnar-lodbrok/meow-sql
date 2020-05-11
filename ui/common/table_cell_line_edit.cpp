#include "table_cell_line_edit.h"

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

     _pushButton = new QPushButton(tr("..."));
     _pushButton->setMaximumWidth(
        _pushButton->fontMetrics().boundingRect(_pushButton->text())
            .width() + 10
     );
     _pushButton->setToolTip(tr("Edit text in popup editor ..."));

     layout->addWidget(_lineEdit, 10);
     layout->addWidget(_pushButton, 1);

     setLayout(layout);
}

} // namespace ui
} // namespace meow
