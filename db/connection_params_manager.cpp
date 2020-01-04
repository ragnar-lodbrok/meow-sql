#include "connection_params_manager.h"
#include <QDebug>
#include <QSettings>
#include "helpers/logger.h"

meow::db::ConnectionParamsManager::ConnectionParamsManager()
    :_params(),
     _uniqueParamsId(0)
{

}

// returns index of just added
int meow::db::ConnectionParamsManager::add(const ConnectionParameters &connectionParams)
{
    _params.append(connectionParams);
    return count() - 1;
}

void meow::db::ConnectionParamsManager::removeParamsAndSave(const ConnectionParameters &params)
{
    int indexToRemove = indexOfParams(params);

    if (indexToRemove == -1) {
        return;
    }

    removeSavedParams(params);

    _params.remove(indexToRemove);
}

bool meow::db::ConnectionParamsManager::renameParamsAndSave(
        ConnectionParameters &params,
        const QString& newName)
{

    if (newName.isEmpty()) {
        return false;
    }

    if (params.sessionName() == newName) {
        saveParams(params);
        return true;
    }

    foreach (const meow::db::ConnectionParameters &itParams, _params) {
        if (itParams.sessionName() == newName) {
            return false;
        }
    }

    params.setSessionName(newName);
    saveParams(params);

    return true;
}


void meow::db::ConnectionParamsManager::updateAndSaveParamsAt(
        int index,
        const ConnectionParameters &connectionParams)
{
    _params.replace(index, connectionParams);
    saveParams(connectionParams);
}

void meow::db::ConnectionParamsManager::load()
{

    // Listening: Sylosis - Where The Wolves Come To Die

    QSettings settings;

    settings.beginGroup("db");
        settings.beginGroup("connection_params_manager");
            QStringList paramIds = settings.childGroups();

            foreach (const QString &paramId, paramIds) {
                settings.beginGroup(paramId);

                unsigned id = paramId.toInt();
                if (_uniqueParamsId < id) {
                    _uniqueParamsId = id;
                }
                if (id == 0) {
                    id = ++_uniqueParamsId;
                }

                ConnectionParameters loadedParams(this);
                loadedParams.setId(id);
                loadedParams.setSessionName(
                            settings.value("sessionName", "").toString());
                int networkType = settings.value("networkType",
                                                 static_cast<int>(loadedParams.port())).toInt();
                loadedParams.setNetworkType(static_cast<NetworkType>(networkType));
                loadedParams.setHostName(
                            settings.value("hostName", loadedParams.hostName()).toString());
                loadedParams.setFileName(
                            settings.value("fileName", loadedParams.fileName()).toString());
                loadedParams.setUserName(
                            settings.value("userName", loadedParams.userName()).toString());
                loadedParams.setPassword(
                            settings.value("password", loadedParams.password()).toString());
                loadedParams.setDatabases(
                            settings.value("databases", loadedParams.databases()).toString());
                loadedParams.setLoginPrompt(
                            settings.value("isLoginPrompt", loadedParams.isLoginPrompt()).toBool());
                loadedParams.setPort(
                            settings.value("port", loadedParams.port()).toInt());
                add(loadedParams);

                settings.endGroup();
            }

        settings.endGroup();
    settings.endGroup();

    meowLogDebug() << "Loaded params: " << count();

}

void meow::db::ConnectionParamsManager::saveParams(const ConnectionParameters & params)
{
    QSettings settings;

    settings.beginGroup("db");
        settings.beginGroup("connection_params_manager");
            settings.beginGroup( QString::number(params.id()) );

                settings.setValue("sessionName", params.sessionName());
                settings.setValue("networkType", static_cast<int>(params.networkType()));
                settings.setValue("hostName", params.hostName());
                settings.setValue("fileName", params.fileName());
                settings.setValue("userName", params.userName());
                settings.setValue("password", params.password()); // TODO: encrypt
                settings.setValue("databases", params.databases());
                settings.setValue("isLoginPrompt", params.isLoginPrompt());
                settings.setValue("port", params.port());

            settings.endGroup();
        settings.endGroup();
    settings.endGroup();
}

void meow::db::ConnectionParamsManager::removeSavedParams(const ConnectionParameters & params)
{

    QSettings settings;

    settings.beginGroup("db");
        settings.beginGroup("connection_params_manager");

            settings.remove( QString::number(params.id()) );

        settings.endGroup();
    settings.endGroup();
}

int meow::db::ConnectionParamsManager::count() const
{
    return _params.size();
}

const meow::db::ConnectionParameters& meow::db::ConnectionParamsManager::at(int index) const
{
    return _params.at(index);
}

meow::db::ConnectionParameters& meow::db::ConnectionParamsManager::at(int index)
{
    return _params[index];
}

const meow::db::ConnectionParameters& meow::db::ConnectionParamsManager::createNewParams()
{
    ConnectionParameters newParams(this);

    newParams.setSessionName(findNextFreeSessionName());
    newParams.setId(++_uniqueParamsId);
    int addedIndex = add(newParams);
    saveParams(newParams);

    return at(addedIndex);
}

int meow::db::ConnectionParamsManager::indexOfParams(const ConnectionParameters & params)
{
    int index = 0;
    foreach (const meow::db::ConnectionParameters &itParams, _params) {
        if (itParams.id() == params.id()) {
            return index;
        }
        ++index;
    }
    return -1;
}

QString meow::db::ConnectionParamsManager::findNextFreeSessionName()
{
    QStringList unnamedSessionNames;

    foreach (const meow::db::ConnectionParameters &itParams, _params) {
        if (itParams.sessionName().startsWith("Unnamed")) {
            unnamedSessionNames << itParams.sessionName();
        }
    }

    QString freeName("Unnamed");

    int i=0;
    while (unnamedSessionNames.contains(freeName)) {
        freeName = QString("Unnamed-%1").arg(++i);
    }

    return freeName;
}
