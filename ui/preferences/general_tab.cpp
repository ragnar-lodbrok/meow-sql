#include "general_tab.h"
#include "ui/presenters/preferences_presenter.h"

namespace meow {
namespace ui {
namespace preferences {

GeneralTab::GeneralTab(presenters::PreferencesPresenter * presenter,
                       QWidget *parent)
    : QWidget(parent)
    , _presenter(presenter)
{
    createWidgets();
    fillDataFromPresenter();

    connect(_presenter,
            &presenters::PreferencesPresenter::applicationLanguageChanged,
            this,
            &GeneralTab::onLanguagePresenterChanged);
}

void GeneralTab::createWidgets()
{
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setColumnMinimumWidth(0, 150);
    mainLayout->setColumnStretch(1, 2);
    mainLayout->setAlignment(Qt::AlignTop);

    int row = 0;

    // Language ----------------------------------------------------------------
    _languageLabel = new QLabel(tr("Application language: *"));
    mainLayout->addWidget(_languageLabel, row, 0);
    _languageComboBox = new QComboBox();
    _languageLabel->setBuddy(_languageComboBox);

    mainLayout->addWidget(_languageComboBox, row, 1);
    connect(_languageComboBox,
        static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
        this, &GeneralTab::onLanguageComboboxIndexChanged);
    row++;

    this->setLayout(mainLayout);
}

void GeneralTab::fillDataFromPresenter()
{
    _languageComboBox->blockSignals(true);
    _languageComboBox->clear();
    std::map<QString, QString> languages = _presenter->applicationLanguages();
    QString currentLanguageCode = _presenter->applicationLanguageCode();
    int languageIndex = 0;
    for (const auto& language : languages) {
        _languageComboBox->addItem(language.second, language.first);
        if (language.first == currentLanguageCode) {
            _languageComboBox->setCurrentIndex(languageIndex);
        }
        ++languageIndex;
    }
    _languageComboBox->blockSignals(false);
}

void GeneralTab::onLanguageComboboxIndexChanged(int index)
{
    QString languageCode = _languageComboBox->itemData(index).toString();
    if (!languageCode.isEmpty()) {
        _presenter->setApplicationLanguage(languageCode);
    }
}

void GeneralTab::onLanguagePresenterChanged()
{
    int index = _languageComboBox->findData(
                    _presenter->applicationLanguageCode());
    if (index != -1) {
        _languageComboBox->setCurrentIndex(index);
    }
}

} // namespace preferences
} // namespace ui
} // namespace meow
