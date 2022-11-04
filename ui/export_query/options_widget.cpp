#include "options_widget.h"
#include "ui/presenters/export_query_presenter.h"

namespace meow {
namespace ui {
namespace export_query {

OptionsWidget::OptionsWidget(presenters::ExportQueryPresenter * presenter,
                             QWidget *parent)
    : QWidget(parent)
    , _presenter(presenter)
{
    createWidgets();
    fillDataFromPresenter();
}

void OptionsWidget::fillDataFromPresenter()
{
    _separatorEdit->setEnabled(_presenter->isOptionEditableFieldSeparator());
    _encloserEdit->setEnabled(_presenter->isOptionEditableEncloser());
    _terminatorEdit->setEnabled(_presenter->isOptionEditableLineTerminator());
    _nullValueEdit->setEnabled(_presenter->isOptionEditableNullValue());

    _includeColumnNamesCheckbox->setEnabled(
                _presenter->isOptionEditableIncludeColumnNames());
    _includeSQLQueryCheckbox->setEnabled(
                _presenter->isOptionEditableIncludeSQLQuery());
    _removeLinebreaksCheckbox->setEnabled(
                _presenter->isOptionEditableRemoveLineBreaksFromContents());
    _includeAutoIncrementColumnCheckbox->setEnabled(
                _presenter->isOptionEditableIncludeAutoIncrementColumn());

    _separatorEdit->blockSignals(true);
    _separatorEdit->setText(_presenter->optionFieldSeparator());
    _separatorEdit->blockSignals(false);

    _encloserEdit->blockSignals(true);
    _encloserEdit->setText(_presenter->optionEncloser());
    _encloserEdit->blockSignals(false);

    _terminatorEdit->blockSignals(true);
    _terminatorEdit->setText(_presenter->optionLineTerminator());
    _terminatorEdit->blockSignals(false);

    _nullValueEdit->blockSignals(true);
    _nullValueEdit->setText(_presenter->optionNullValue());
    _nullValueEdit->blockSignals(false);

    _includeColumnNamesCheckbox->blockSignals(true);
    _includeColumnNamesCheckbox->setChecked(
                _presenter->optionBoolIncludeColumnNames());
    _includeColumnNamesCheckbox->blockSignals(false);

    _includeSQLQueryCheckbox->blockSignals(true);
    _includeSQLQueryCheckbox->setChecked(
                _presenter->optionBoolIncludeSQLQuery());
    _includeSQLQueryCheckbox->blockSignals(false);

    _removeLinebreaksCheckbox->blockSignals(true);
    _removeLinebreaksCheckbox->setChecked(
                _presenter->optionBoolRemoveLineBreaksFromContents());
    _removeLinebreaksCheckbox->blockSignals(false);

    _includeAutoIncrementColumnCheckbox->blockSignals(true);
    _includeAutoIncrementColumnCheckbox->setChecked(
                _presenter->optionBoolIncludeAutoIncrementColumn());
    _includeAutoIncrementColumnCheckbox->blockSignals(false);

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

    connect(_includeColumnNamesCheckbox, &QCheckBox::stateChanged,
            this, &OptionsWidget::onCheckboxStateChanged);
    connect(_includeAutoIncrementColumnCheckbox, &QCheckBox::stateChanged,
            this, &OptionsWidget::onCheckboxStateChanged);
    connect(_includeSQLQueryCheckbox, &QCheckBox::stateChanged,
            this, &OptionsWidget::onCheckboxStateChanged);
    connect(_removeLinebreaksCheckbox, &QCheckBox::stateChanged,
            this, &OptionsWidget::onCheckboxStateChanged);


    _separatorLabel = new QLabel(tr("Field separator:"));
    mainGroupLayout->addWidget(_separatorLabel, 0, 1);

    _encloserLabel = new QLabel(tr("Encloser:"));
    mainGroupLayout->addWidget(_encloserLabel, 1, 1);

    _terminatorLabel = new QLabel(tr("Line terminator:"));
    mainGroupLayout->addWidget(_terminatorLabel, 2, 1);

    _nullValueLabel = new QLabel(tr("NULL value:"));
    mainGroupLayout->addWidget(_nullValueLabel, 3, 1);

    _separatorEdit = createLineEditWithAction();
    mainGroupLayout->addWidget(_separatorEdit, 0, 2);

    _encloserEdit = createLineEditWithAction();
    mainGroupLayout->addWidget(_encloserEdit, 1, 2);

    _terminatorEdit = createLineEditWithAction();
    mainGroupLayout->addWidget(_terminatorEdit, 2, 2);

    _nullValueEdit = createLineEditWithAction();
    mainGroupLayout->addWidget(_nullValueEdit, 3, 2);

    connect(_separatorEdit, &QLineEdit::textChanged,
            this, &OptionsWidget::onLineEditTextChanged);
    connect(_encloserEdit, &QLineEdit::textChanged,
            this, &OptionsWidget::onLineEditTextChanged);
    connect(_terminatorEdit, &QLineEdit::textChanged,
            this, &OptionsWidget::onLineEditTextChanged);
    connect(_nullValueEdit, &QLineEdit::textChanged,
            this, &OptionsWidget::onLineEditTextChanged);

    mainGroupLayout->setColumnStretch(0, 2);
    mainGroupLayout->setColumnStretch(1, 0);
    mainGroupLayout->setColumnStretch(2, 1);
    mainGroupLayout->setAlignment(Qt::AlignTop);

    _groupBox->setLayout(mainGroupLayout);
}

QLineEdit * OptionsWidget::createLineEditWithAction()
{
    QLineEdit * edit = new QLineEdit;

    QAction * action = edit->addAction(
                QIcon(":/icons/dropdown_highlight.png"),
                QLineEdit::TrailingPosition);
    connect(action, &QAction::triggered,
            this, &OptionsWidget::onLineEditAction);

    return edit;
}

void OptionsWidget::onLineEditTextChanged()
{
    if (sender() == _separatorEdit) {
        _presenter->setOptionFieldSeparator(_separatorEdit->text());
    } else if (sender() == _encloserEdit) {
        _presenter->setOptionEncloser(_encloserEdit->text());
    } else if (sender() == _terminatorEdit) {
        _presenter->setOptionLineTerminator(_terminatorEdit->text());
    } else if (sender() == _nullValueEdit) {
        _presenter->setOptionNullValue(_nullValueEdit->text());
    }
}

void OptionsWidget::onCheckboxStateChanged()
{
    if (sender() == _includeColumnNamesCheckbox) {
        _presenter->setOptionIncludeColumnNames(
                    _includeColumnNamesCheckbox->isChecked());

    } else if (sender() == _includeAutoIncrementColumnCheckbox) {
        _presenter->setOptionIncludeAutoIncrementColumn(
                    _includeAutoIncrementColumnCheckbox->isChecked());

    } else if (sender() == _includeSQLQueryCheckbox) {
        _presenter->setOptionIncludeSQLQuery(
                    _includeSQLQueryCheckbox->isChecked());

    } else if (sender() == _removeLinebreaksCheckbox) {
        _presenter->setOptionRemoveLineBreaksFromContents(
                    _removeLinebreaksCheckbox->isChecked());
    }
}

void OptionsWidget::onLineEditAction()
{
    QLineEdit * lineEdit = static_cast<QLineEdit *>(sender()->parent());

    QVector<QPair<QString, QString>> options = _presenter->optionValues();

    QMenu * menu = new QMenu;

    for (const QPair<QString, QString> & pair : options) {
        QString name = pair.second;
        QString value = pair.first;

        if (name == "-") {
            menu->addSeparator();
        } else {
            QAction * action = new QAction(name, menu);
            action->setData(value);
            menu->addAction(action);
        }
    }

    int menuXPos = lineEdit->width() - menu->sizeHint().width();
    int menuYPos = lineEdit->height() - 1;


    QAction * executedAction
            = menu->exec(lineEdit->mapToGlobal(
                                 QPoint(menuXPos, menuYPos)));

    if (executedAction) {
        lineEdit->setText(executedAction->data().toString());
    }

    delete menu;
}

} // namespace export_query
} // namespace ui
} // namespace meow
