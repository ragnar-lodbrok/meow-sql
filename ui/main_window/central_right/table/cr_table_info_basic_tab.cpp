#include "cr_table_info_basic_tab.h"
#include "models/forms/table_info_form.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

BasicTab::BasicTab(models::forms::TableInfoForm * form,
                   QWidget * parent)
    : QWidget(parent),
      _tableForm(form)
{
    Q_CHECK_PTR(form);
    createWidgets();
}

void BasicTab::refreshData()
{
    fillDataFromForm();
}

void BasicTab::onBeforeEntityEditing()
{
    _nameEdit->setFocus();
}

void BasicTab::createWidgets()
{
    _mainGridLayout = new QGridLayout();
    int row = 0;

    _nameLabel = new QLabel(tr("Name:"));
    _mainGridLayout->addWidget(_nameLabel, row, 0);

    _nameEdit = new QLineEdit();
    _nameEdit->setMaxLength(64);
    connect(_nameEdit, &QLineEdit::textEdited,
            [=](const QString &name) {
                _tableForm->setTableName(name);
            });
    _nameLabel->setBuddy(_nameEdit);
    _mainGridLayout->addWidget(_nameEdit, row, 1);

    ++row;

    _commentLabel = new QLabel(tr("Comment:"));
    _mainGridLayout->addWidget(_commentLabel, row, 0, Qt::AlignTop);

    _commentEdit = new QPlainTextEdit();
    // TODO: max len
    connect(_commentEdit, &QPlainTextEdit::textChanged,
            [=]() {
                _tableForm->setTableComment(_commentEdit->toPlainText());
            });
    _commentLabel->setBuddy(_commentEdit);
    _mainGridLayout->addWidget(_commentEdit, row, 1);

    _mainGridLayout->setColumnMinimumWidth(0, 100);
    _mainGridLayout->setColumnStretch(0, 0);
    _mainGridLayout->setColumnStretch(1, 5);
    _mainGridLayout->setAlignment(Qt::AlignTop);
    this->setLayout(_mainGridLayout);
}

void BasicTab::fillDataFromForm()
{
    _nameEdit->setText(_tableForm->tableName());

    _commentEdit->blockSignals(true);
    _commentEdit->setPlainText(_tableForm->tableComment());
    _commentEdit->blockSignals(false);
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
