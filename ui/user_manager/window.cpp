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
    , _form(session->connection()->userManager())
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
    //mainLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(mainLayout);

    _splitter = new QSplitter(this);
    _splitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_splitter);

    _leftWidget = new LeftWidget(&_form);
    _leftWidget->setMinimumSize(QSize(300, 400));

    _rightWidget = new RightWidget(&_form);
    _rightWidget->setMinimumSize(QSize(470, 400));

    _splitter->addWidget(_leftWidget);
    _splitter->setStretchFactor(0, 1);
    _splitter->addWidget(_rightWidget);
    _splitter->setStretchFactor(1, 2);

    _warningLabel = new QLabel();
    _warningLabel->setWordWrap(true);
    mainLayout->addWidget(_warningLabel);

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

} // namespace user_manager
} // namespace ui
} // namespace
