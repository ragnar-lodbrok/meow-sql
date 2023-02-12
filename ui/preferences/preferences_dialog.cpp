#include "preferences_dialog.h"
#include "general_tab.h"

namespace meow {
namespace ui {
namespace preferences {

Dialog::Dialog(settings::Core * settings)
    : _presenter(settings)
{
    setMinimumSize(400, 350);
    setWindowTitle(tr("Preferences"));

    createWidgets();
    validateControls();

    resize(700, 500);

    connect(&_presenter, &presenters::PreferencesPresenter::modifiedChanged,
            this, &Dialog::onPresenterModifiedChanged);
}

void Dialog::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    _rootTabs = new QTabWidget();

    createTabs();

    mainLayout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

    QWidget * bottomWidget = new QWidget();

    _okButton = new QPushButton(tr("OK"));
    _cancelButton = new QPushButton(tr("Cancel"));
    _applyButton = new QPushButton(tr("Apply"));

    _okButton->setDefault(true);

    connect(_okButton, &QAbstractButton::clicked,
            this, &Dialog::onOkButtonClicked);
    connect(_cancelButton, &QAbstractButton::clicked,
            this, &Dialog::onCancelButtonClicked);
    connect(_applyButton, &QAbstractButton::clicked,
            this, &Dialog::onApplyButtonClicked);

    QHBoxLayout * buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(_okButton);
    buttonsLayout->addWidget(_cancelButton);
    buttonsLayout->addWidget(_applyButton);

    bottomWidget->setLayout(buttonsLayout);

    mainLayout->addWidget(bottomWidget);
}

void Dialog::createTabs()
{
    GeneralTab * generalTab = new GeneralTab(&_presenter);
    _rootTabs->addTab(generalTab, QIcon(":/icons/trigger.png"), tr("General"));

}

void Dialog::validateControls()
{
    _applyButton->setEnabled(_presenter.isApplyEnabled());
}

void Dialog::onOkButtonClicked()
{
    _presenter.apply();
    accept();
}

void Dialog::onCancelButtonClicked()
{
    reject();
}

void Dialog::onApplyButtonClicked()
{
    _presenter.apply();
}

void Dialog::onPresenterModifiedChanged()
{
    validateControls();
}

} // namespace preferences
} // namespace ui
} // namespace meow
