#include "connection_params_manager.h"
#include <QDebug>
#include <QSettings>

meow::db::ConnectionParamsManager::ConnectionParamsManager()
    :_params()
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

    qDebug() << "Remove params: " << indexToRemove;

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
    // need this shit bc params are identified by name and need to be unique

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

    removeSavedParams(params);
    params.setSessionName(newName);
    saveParams(params);

    return true;
}


void meow::db::ConnectionParamsManager::updateAndSaveParamsAt(int index, const ConnectionParameters &connectionParams)
{
    _params.replace(index, connectionParams);
    saveParams(connectionParams);
    qDebug() << "Saved: " << connectionParams.sessionName();
}

void meow::db::ConnectionParamsManager::load()
{

    QSettings settings;

    settings.beginGroup("db");
        settings.beginGroup("connection_params_manager");
            QStringList sessionNames = settings.childGroups();

            foreach (const QString &sessionName, sessionNames) {
                settings.beginGroup(sessionName);

                ConnectionParameters loadedParams(this);
                loadedParams.setSessionName(sessionName);
                int networkType = settings.value("networkType", static_cast<int>(loadedParams.port())).toInt();
                loadedParams.setNetworkType(static_cast<NetworkType>(networkType));
                loadedParams.setHostName(settings.value("hostName", loadedParams.hostName()).toString());
                loadedParams.setUserName(settings.value("userName", loadedParams.userName()).toString());
                loadedParams.setPassword(settings.value("password", loadedParams.password()).toString());
                loadedParams.setLoginPrompt(settings.value("isLoginPrompt", loadedParams.isLoginPrompt()).toBool());
                loadedParams.setPort(settings.value("port", loadedParams.port()).toInt());
                add(loadedParams);

                settings.endGroup();
            }

        settings.endGroup();
    settings.endGroup();

    qDebug() << "Loaded params: " << count();

}

void meow::db::ConnectionParamsManager::saveParams(const ConnectionParameters & params)
{
    QSettings settings;

    settings.beginGroup("db");
        settings.beginGroup("connection_params_manager");
            settings.beginGroup(params.sessionName());

                settings.setValue("networkType", static_cast<int>(params.networkType()));
                settings.setValue("hostName", params.hostName());
                settings.setValue("userName", params.userName());
                settings.setValue("password", params.password()); // TODO: encrypt
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

            settings.remove(params.sessionName());

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
    int addedIndex = add(newParams);
    saveParams(newParams);

    return at(addedIndex);
}

int meow::db::ConnectionParamsManager::indexOfParams(const ConnectionParameters & params)
{
    int index = 0;
    foreach (const meow::db::ConnectionParameters &itParams, _params) {
        if (itParams.sessionName() == params.sessionName()) {
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
