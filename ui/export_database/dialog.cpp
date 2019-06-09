#include "dialog.h"
#include "top_widget.h"
#include "bottom_widget.h"
#include "models/forms/export_database_form.h"

namespace meow {
namespace ui {
namespace export_database {

Dialog::Dialog(models::forms::ExportDatabaseForm * form)
    : QDialog(nullptr, Qt::WindowCloseButtonHint),
      _form(form)
{
    setMinimumSize(320, 200);
    setWindowTitle(tr("Export database as SQL"));

    createWidgets();
    fillDataFromForm();

    resize(600, 300);
}

void Dialog::createWidgets()
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_mainLayout);

    _topWidget = new TopWidget(_form, this);
    _bottomWidget = new BottomWidget(this);

    connect(_bottomWidget->cancelButton(),
            &QAbstractButton::clicked,
            this,
            &Dialog::onCancel
    );

    connect(_bottomWidget->exportButton(),
            &QAbstractButton::clicked,
            this,
            &Dialog::onExport
    );


    _mainLayout->addWidget(_topWidget, 1);
    _mainLayout->addWidget(_bottomWidget, 0, Qt::AlignBottom);
}

void Dialog::fillDataFromForm()
{
    _topWidget->fillDataFromForm();
}

void Dialog::onCancel()
{
    reject();
}

void Dialog::onExport()
{

}

} // namespace export_database
} // namespace ui
} // namespace meow
