#ifndef MEOW_UI_PRESENTERS_PREFERENCES_PRESENTER_H
#define MEOW_UI_PRESENTERS_PREFERENCES_PRESENTER_H

#include <QObject>
#include "map"
#include "settings/settings_core.h"

namespace meow {
namespace ui {
namespace presenters {

using IdLanguagePair = QPair<QString, QString>;

class PreferencesPresenter : public QObject
{
    Q_OBJECT
public:
    explicit PreferencesPresenter(settings::Core * settings);

    std::map<QString, QString> applicationLanguages() const;
    QString applicationLanguageCode();
    void setApplicationLanguage(const QString & languageCode);

    void setModified(bool modified);

    bool isApplyEnabled() const {
        return _modified;
    }

    void apply();

    Q_SIGNAL void applicationLanguageChanged();
    Q_SIGNAL void modifiedChanged();
private:
    settings::Core * _settings;
    std::unique_ptr<settings::UserPreferences> _userPreferencesCopy;
    bool _modified;
    bool _requiresRestart;
};

} // namespace presenters
} // namespace ui
} // namespace meow

#endif // MEOW_UI_PRESENTERS_PREFERENCES_PRESENTER_H
