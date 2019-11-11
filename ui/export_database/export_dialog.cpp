#include "export_dialog.h"
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
    setMinimumSize(320, 300);
    setWindowTitle(tr("Export database as SQL"));

    createWidgets();
    fillDataFromForm();

    resize(800, 400);
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

    connect(_form,
            &models::forms::ExportDatabaseForm::finished,
            this,
            &Dialog::exportFinished);

    connect(_form,
            &models::forms::ExportDatabaseForm::progressMessage,
            _topWidget,
            &TopWidget::appendToResults);


    _mainLayout->addWidget(_topWidget, 1);
    _mainLayout->addWidget(_bottomWidget, 0, Qt::AlignBottom);

}

void Dialog::fillDataFromForm()
{
    _topWidget->fillDataFromForm();
}

void Dialog::onCancel()
{
    if (_form->cancelExport() == false) {
        // close if was not running
        reject();
    }
}

void Dialog::onExport()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    _bottomWidget->exportButton()->setEnabled(false);
    _topWidget->setInputsEnabled(false);
    _topWidget->clearResults();
    _form->startExport();
}

void Dialog::exportFinished(bool success)
{
    if (success == false) {
        QMessageBox msgBox;
        msgBox.setText(tr("Export failed.\nSee logs for details."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
    _bottomWidget->exportButton()->setEnabled(true);
    _topWidget->setInputsEnabled(true);
    QApplication::restoreOverrideCursor();
}

} // namespace export_database
} // namespace ui
} // namespace meow
