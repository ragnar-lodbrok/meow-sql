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

void BasicTab::createWidgets()
{
    _mainGridLayout = new QGridLayout();
    int row = 0;

    _nameLabel = new QLabel(tr("Name:"));
    _mainGridLayout->addWidget(_nameLabel, row, 0);

    _nameEdit = new QLineEdit();
    _nameEdit->setMaxLength(64);
    _nameEdit->setReadOnly(true);
    _nameEdit->setToolTip("Readonly"); // temp
    _nameLabel->setBuddy(_nameEdit);
    _mainGridLayout->addWidget(_nameEdit, row, 1);

    ++row;

    _commentLabel = new QLabel(tr("Comment:"));
    _mainGridLayout->addWidget(_commentLabel, row, 0, Qt::AlignTop);

    _commentEdit = new QPlainTextEdit();
    // TODO: max len
    _commentEdit->setReadOnly(true);
    _commentLabel->setToolTip("Readonly");
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
    _commentEdit->setPlainText(_tableForm->tableComment());
}

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
