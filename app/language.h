#ifndef MEOW_APP_LANGUAGE_H
#define MEOW_APP_LANGUAGE_H

#include <QString>
#include <QVector>
#include <QObject>

namespace meow {

using LanguageCode = QString;

struct Language
{
    LanguageCode code;
    QString name;
    QString nameTranslated;
};

static inline LanguageCode defaultLanguage()
{
    return "en";
}

static inline QVector<Language> allLanguages()
{
    static QVector<Language> langs = {
        {"bg", "Bulgarian",  QObject::tr("Bulgarian")},
        {"zh", "Chinese",    QObject::tr("Chinese")},
        {"cs", "Czech",      QObject::tr("Czech")},
        {"en", "English",    QObject::tr("English")},
        {"fr", "French",     QObject::tr("French")},
        {"de", "German",     QObject::tr("German")},
        {"hu", "Hungarian",  QObject::tr("Hungarian")},
        {"it", "Italian",    QObject::tr("Italian")},
        {"ko", "Korean",     QObject::tr("Korean")},
        {"pt", "Portuguese", QObject::tr("Portuguese")},
        {"ro", "Romanian",   QObject::tr("Romanian")},
        {"ru", "Russian",    QObject::tr("Russian")},
        {"es", "Spanish",    QObject::tr("Spanish")},
        {"sv", "Swedish",    QObject::tr("Swedish")},
    };

    return langs;
}

} // namespace meow

#endif // MEOW_APP_LANGUAGE_H
