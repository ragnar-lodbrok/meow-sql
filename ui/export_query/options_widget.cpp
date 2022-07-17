#include "options_widget.h"

namespace meow {
namespace ui {
namespace export_query {

OptionsWidget::OptionsWidget(QWidget *parent)
    : QWidget(parent)
{
    createWidgets();
}

void OptionsWidget::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _groupBox = new QGroupBox(tr("Options"));
    _groupBox->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(_groupBox);

    QGridLayout * mainGroupLayout = new QGridLayout();

    _includeColumnNamesCheckbox = new QCheckBox(tr("Include column names"));
    mainGroupLayout->addWidget(_includeColumnNamesCheckbox, 0, 0);

    _includeAutoIncrementColumnCheckbox
            = new QCheckBox(tr("Include auto increment column"));
    mainGroupLayout->addWidget(_includeAutoIncrementColumnCheckbox, 1, 0);

    _includeSQLQueryCheckbox
            = new QCheckBox(tr("Include SQL query"));
    mainGroupLayout->addWidget(_includeSQLQueryCheckbox, 2, 0);

    _removeLinebreaksCheckbox
            = new QCheckBox(tr("Remove linebreaks from contents"));
    mainGroupLayout->addWidget(_removeLinebreaksCheckbox, 3, 0);

    _separatorLabel = new QLabel(tr("Field separator:"));
    mainGroupLayout->addWidget(_separatorLabel, 0, 1);

    _encloserLabel = new QLabel(tr("Encloser:"));
    mainGroupLayout->addWidget(_encloserLabel, 1, 1);

    _terminatorLabel = new QLabel(tr("Line terminator:"));
    mainGroupLayout->addWidget(_terminatorLabel, 2, 1);

    _nullValueLabel = new QLabel(tr("NULL value:"));
    mainGroupLayout->addWidget(_nullValueLabel, 3, 1);

    _separatorEdit = new QLineEdit;
    mainGroupLayout->addWidget(_separatorEdit, 0, 2);

    _encloserEdit = new QLineEdit;
    mainGroupLayout->addWidget(_encloserEdit, 1, 2);

    _terminatorEdit = new QLineEdit;
    mainGroupLayout->addWidget(_terminatorEdit, 2, 2);

    _nullValueEdit = new QLineEdit;
    mainGroupLayout->addWidget(_nullValueEdit, 3, 2);

    mainGroupLayout->setColumnStretch(0, 2);
    mainGroupLayout->setColumnStretch(1, 0);
    mainGroupLayout->setColumnStretch(2, 1);
    mainGroupLayout->setAlignment(Qt::AlignTop);

    _groupBox->setLayout(mainGroupLayout);
}

} // namespace export_query
} // namespace ui
} // namespace meow
