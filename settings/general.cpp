#include "general.h"
#include <QSettings>

namespace meow {
namespace settings {

static const char LANGUAGE_SETTINGS_KEY[] = "settings/general/language_code";

General::General()
{

}

void General::copyDataTo(General * copy) const
{
    // silent copy
    copy->_language = this->_language;
}

void General::setDataFrom(General * source)
{
    setLanguage(source->language());
}

void General::setLanguage(LanguageCode lang)
{
    if (_language != lang) {
        _language = lang;
        emit languageChanged(lang);
    }
}

void General::save()
{
    QSettings settings;
    settings.setValue(LANGUAGE_SETTINGS_KEY, _language);
}

void General::load()
{
    QSettings settings;
    _language = settings.value(LANGUAGE_SETTINGS_KEY,
                               defaultLanguage()).toString();
}

} // namespace meow
} // namespace settings
