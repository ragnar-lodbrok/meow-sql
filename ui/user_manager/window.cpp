#include "window.h"
#include "left_widget.h"
#include "right_widget.h"
#include "ui/main_window/main_window.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace user_manager {

Window::Window(main_window::Window * mainWindow, db::SessionEntity * session)
    : QDialog(mainWindow, Qt::WindowCloseButtonHint)
    , _form(session)
    , _mainWindow(mainWindow)
    , _session(session)
{

    createWidgets();

    setMinimumSize(QSize(800, 400));
    setWindowTitle(tr("User manager"));

    connect(&_form, &models::forms::UserManagementForm::selectedUserChanged,
            this, &Window::onSelectedUserChanged);

    onSelectedUserChanged();

    //loadGeometryFromSettings(); // TODO
}

Window::~Window()
{
    //saveGeometryToSettings(); // TODO
}

void Window::loadData()
{
    try {
        _leftWidget->loadData();
    } catch (meow::db::Exception & ex) {
        showErrorMessage(ex.message());
    }
}

void Window::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    _splitter = new QSplitter(this);
    _splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_splitter, 1);

    _leftWidget = new LeftWidget(&_form);
    _leftWidget->setMinimumSize(QSize(300, 400));

    _rightWidget = new RightWidget(&_form, this);
    _rightWidget->setMinimumSize(QSize(470, 400));

    _splitter->addWidget(_leftWidget);
    _splitter->setStretchFactor(0, 0);
    _splitter->addWidget(_rightWidget);
    _splitter->setStretchFactor(1, 1);

    // Bottom ------------------------------------------------------------------

    QHBoxLayout * bottomLayout = new QHBoxLayout();
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(bottomLayout);

    _warningLabel = new QLabel();
    _warningLabel->setWordWrap(true);
    bottomLayout->addWidget(_warningLabel, 0);

    bottomLayout->addStretch(1);

    _saveButton = new QPushButton(QIcon(":/icons/disk.png"),
                                     tr("Save"));
    bottomLayout->addWidget(_saveButton, 0, Qt::AlignRight);
    connect(_saveButton,
            &QAbstractButton::clicked,
            this,
            &Window::onSaveClicked
    );

    _discardButton = new QPushButton(QIcon(":/icons/arrow_undo.png"),
                                     tr("Discard"));
    bottomLayout->addWidget(_discardButton, 0, Qt::AlignRight);
    connect(_discardButton,
            &QAbstractButton::clicked,
            this,
            &Window::onDiscardClicked
    );
}

void Window::validateControls()
{
    // TODO
    _discardButton->setEnabled(_form.hasUnsavedChanges());
    _saveButton->setEnabled(_form.hasUnsavedChanges());
}

void Window::showErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void Window::onSelectedUserChanged()
{
    _warningLabel->setText(_form.userWarningMessage());
    _rightWidget->fillDataFromForm();
}

void Window::onSaveClicked()
{
    try {
        _form.save();
    } catch(meow::db::Exception & ex) {
        showErrorMessage(ex.message());
    }
}

void Window::onDiscardClicked()
{
    _form.discard();
}

} // namespace user_manager
} // namespace ui
} // namespace
