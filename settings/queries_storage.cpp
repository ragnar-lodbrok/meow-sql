#include "queries_storage.h"
#include "db/user_query/user_query.h"
#include "helpers/logger.h"
#include "db/connections_manager.h"
#include "db/user_queries_manager.h"
#include "app/app.h"
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QTextStream>
#include <QSettings>
#include <QDebug>
#include <algorithm>

// Note: we add QCoreApplication::applicationName() to all file names to
// handle other apps with same organizationName(). Need to change org name
// from "ragnar-lodbrok" to "MeowSQL" to have own dir and then remove prefix.

namespace meow {
namespace settings {

const char settingsFilename[] = "Tabs";
const int64_t maxQueryLength = 1*1024*1024; // 1 Mib

QueriesStorage::QueriesStorage()
{
    QString backupPath = this->backupPath();
    QDir backupDir;
    if (!backupDir.exists(backupPath)) {
        backupDir.mkpath(backupPath);
    }

}

void QueriesStorage::save(const std::vector<db::UserQuery *> & queries)
{
    QStringList prevQueryIds = loadQueryIds();

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.beginGroup("userQueries");

    int index = 0;
    for (db::UserQuery * query : queries) {

        QString filename = backupFileName(query->uniqueId());

        backup(query);

        settings.beginGroup(query->uniqueId());
        // save index for correct order restore:
        settings.setValue("index", QString::number(index));
        settings.setValue("BackupFilename",
                          QDir::toNativeSeparators(filename));
        settings.endGroup();

        ++index;
    }

    for (const QString & prevId : prevQueryIds) {
        bool prevIdExist = std::find_if(
            queries.cbegin(), queries.cend(),
            [=](db::UserQuery * query) {
                return query->uniqueId() == prevId;
            }) != queries.cend();
        if (!prevIdExist) {
            settings.remove(prevId);
            QFile::remove(backupFileName(prevId));
        }
    }

    settings.endGroup();
}

std::vector<db::UserQuery *> QueriesStorage::load()
{
    db::UserQueriesManager * queryManager
            = meow::app()->dbConnectionsManager()->userQueriesManager();

    std::vector<db::UserQuery *> queries;

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.beginGroup("userQueries");

    // Note: QSettings doesn't preserve order of groups, so we need to use index
    // We save queries to vector by index, then remove cells that are not filled

    QStringList ids = settings.childGroups();

    queries.resize(ids.count()); // fills with nullptr

    for (const QString & id : ids) {

        if (id.isEmpty()) continue;

        settings.beginGroup(id);
        size_t index = settings.value("index").toInt();
        bool indexIsValid = (index < queries.size());
        QString filename = settings.value("BackupFilename").toString();

        if (filename.isEmpty() || !indexIsValid)  {
            settings.endGroup();
            continue;
        }

        db::UserQuery * query = queryManager->createUserQueryObject();
        query->setUniqueId(id);

        if (queries.at(index) != nullptr) {
            // avoid memory leak if settings is mailformed
            delete queries.at(index);
        }

        queries[index] = query; // save to right pos

        QFile file(filename);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {

            QTextStream in(&file);
            query->setCurrentQueryText(in.read(maxQueryLength));

            // Still continue if no file - may be just empty tab
        }

        settings.endGroup();
    }

    settings.endGroup();

    queries.erase( // erase uninnit queries (should not happen)
                std::remove(queries.begin(), queries.end(), nullptr),
                queries.end());
    return queries;
}

bool QueriesStorage::backup(db::UserQuery * query)
{
    if (query->modifiedButNotSaved() == false) {
        return true;
    }

    QString fileName = backupFileName(query->uniqueId());

    if (query->currentQueryText().isEmpty()) {
        // TODO: if query is empty - remove backup file?
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream stream(&file);
        stream << query->currentQueryText();
        stream.flush();
        file.close();
        query->setModifiedButNotSaved(false); // saved
        return true;
    } else {
        meowLogC(Log::Category::Error) << "Unable to open query "
                                       << "backup file for write: "
                                       << fileName;
        return false;
    }
}

QString QueriesStorage::backupPath() const
{
#ifdef Q_OS_UNIX
    // I prefer ~/.config
    QString rootLocation = QStandardPaths::writableLocation(
                QStandardPaths::ConfigLocation)
            + QDir::separator()
            + QCoreApplication::organizationName();
#else
    // on win/mac it is common to write to App Data/Application Support
    QString rootLocation = QStandardPaths::writableLocation(
                QStandardPaths::AppLocalDataLocation);
#endif
    return rootLocation + QDir::separator()
            + QCoreApplication::applicationName() + "QueriesBackup";
}

QString QueriesStorage::backupFileName(const QString & queryId) const
{
    return backupPath() + QDir::separator() + queryId + ".sql";
}

QStringList QueriesStorage::loadQueryIds()
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       QCoreApplication::organizationName(),
                       QCoreApplication::applicationName() + settingsFilename);

    settings.beginGroup("userQueries");

    QStringList ids = settings.childGroups();

    settings.endGroup();

    return ids;
}

} // namespace meow
} // namespace settings
