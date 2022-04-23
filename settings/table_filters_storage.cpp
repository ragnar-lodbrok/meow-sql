#include "table_filters_storage.h"
#include <cstdint>
#include <QSettings>
#include <QCoreApplication>
#include <QDebug>
#include <QCollator>

namespace meow {
namespace settings {

const char settingsFilename[] = "TableFilters";
const int64_t maxFilterLength = 1*1024*1024; // 1 Mib

TableFiltersStorage::TableFiltersStorage()
{

}


void TableFiltersStorage::setTable(
        const QString & sessionName,
        const QString & databaseName,
        const QString & tableName)
{
    _sessionName = sessionName;
    _databaseName = databaseName;
    _tableName = tableName;
}

QStringList TableFiltersStorage::load()
{
    Q_ASSERT(!_sessionName.isEmpty());
    Q_ASSERT(!_databaseName.isEmpty());
    Q_ASSERT(!_tableName.isEmpty());

    QStringList filters;

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.beginGroup(_sessionName);
    settings.beginGroup(_databaseName);
    settings.beginGroup(_tableName);

    QStringList ids = settings.childKeys();

    QCollator collator;
    collator.setNumericMode(true); // natural order: 1, 2, 10, 20 etc
    std::sort(ids.begin(), ids.end(), collator);

    int count = std::min(ids.size(), TableFiltersStorage::maxCount());
    filters.reserve(count);
    int i = 0;

    for (const QString & id : ids) {
        if (++i > count) break;
        QString filter = settings.value(id).toString();
        if (filter.isEmpty()) continue;
        if (filter.length() > maxFilterLength) {
            filter = filter.left(maxFilterLength);
        }
        filters.push_back(filter);
    }

    settings.endGroup(); // _tableName
    settings.endGroup(); // _databaseName
    settings.endGroup(); // _sessionName

    return filters;
}

void TableFiltersStorage::save(const QStringList & filters)
{
    Q_ASSERT(!_sessionName.isEmpty());
    Q_ASSERT(!_databaseName.isEmpty());
    Q_ASSERT(!_tableName.isEmpty());

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.beginGroup(_sessionName);
    settings.beginGroup(_databaseName);
    settings.beginGroup(_tableName);

    settings.remove(""); // remove all table filters

    int count = std::min(filters.size(), TableFiltersStorage::maxCount());

    for (int i = 0; i < count; ++i) {
        QString filter = filters[i];
        if (filter.length() > maxFilterLength) {
            filter = filter.left(maxFilterLength);
        }
        settings.setValue(QString::number(i), filter);
    }

    settings.endGroup(); // _tableName
    settings.endGroup(); // _databaseName
    settings.endGroup(); // _sessionName
}

} // namespace meow
} // namespace settings

