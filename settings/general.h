#ifndef MEOW_SETTINGS_GENERAL_H
#define MEOW_SETTINGS_GENERAL_H

#include <QObject>
#include "app/language.h"

namespace meow {
namespace settings {

class General : public QObject
{
    Q_OBJECT
public:
    General();

    void copyDataTo(General * copy) const;
    void setDataFrom(General * source);

    LanguageCode language() const {
        return _language;
    }
    void setLanguage(LanguageCode lang);
    Q_SIGNAL void languageChanged(LanguageCode language);

    void load();
    void save();

private:
    LanguageCode _language;
};

} // namespace meow
} // namespace settings

#endif // MEOW_SETTINGS_GENERAL_H
