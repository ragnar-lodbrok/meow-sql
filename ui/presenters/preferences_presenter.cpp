#include "preferences_presenter.h"
#include "settings/settings_core.h"
#include "settings/general.h"
#include <QMessageBox>
#include <QCoreApplication>

namespace meow {
namespace ui {
namespace presenters {

PreferencesPresenter::PreferencesPresenter(settings::Core * settings)
    : QObject(nullptr)
    , _settings(settings)
    , _userPreferencesCopy(_settings->userPreferences()->clone())
    , _modified(false)
    , _requiresRestart(false)
{
    Q_ASSERT(settings != nullptr);

    connect(_userPreferencesCopy->generalSettings(),
            &meow::settings::General::languageChanged,
            this,
            &PreferencesPresenter::applicationLanguageChanged);
}

std::map<QString, QString> PreferencesPresenter::applicationLanguages() const
{
    const QVector<Language> & allLangs = meow::allLanguages();

    std::map<QString, QString> langsPresentation;

    for (const auto& languageIt : allLangs) {
        QString languageName = languageIt.code + ": " + languageIt.name;
        if (languageIt.name != languageIt.nameTranslated) {
            languageName += " - " + languageIt.nameTranslated;
        }
        langsPresentation.emplace(languageIt.code, languageName);
    }

    return langsPresentation;
}

QString PreferencesPresenter::applicationLanguageCode()
{
    return _userPreferencesCopy->generalSettings()->language();
}

void PreferencesPresenter::setApplicationLanguage(const QString & languageCode)
{
    _userPreferencesCopy->generalSettings()->setLanguage(languageCode);
    setModified(true);
    _requiresRestart = true;
}

void PreferencesPresenter::setModified(bool modified)
{
    if (_modified == modified) return;
    _modified = modified;
    emit modifiedChanged();
}

void PreferencesPresenter::apply()
{
    _settings->userPreferences()->setDataFrom(_userPreferencesCopy.get());
    _settings->userPreferences()->save();
    setModified(false);
    if (_requiresRestart) {
        QMessageBox msgBox;
        msgBox.setText(tr("You should restart %1 to apply changed critical"
                          " settings, and to prevent unexpected behaviour.")
                       .arg(qApp->applicationName()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.exec();
        _requiresRestart = false;
    }
}

} // namespace presenters
} // namespace ui
} // namespace meow
