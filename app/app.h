#ifndef MEOW_APP_H
#define MEOW_APP_H

#include "db/connection_params_manager.h"
#include "db/connections_manager.h"

#include "settings/settings_core.h"

#include "actions.h"

#include "log.h"

namespace meow {

class App
{
public:
    App();
    db::ConnectionParamsManager * dbConnectionParamsManager();
    db::ConnectionsManager * dbConnectionsManager();

    meow::settings::Core * settings() { return &_settingsCore; }

    Actions * actions() { return &_actions; }

    Log * log() { return &_log; }

private:
    meow::db::ConnectionParamsManager _dbConnectionParamsManager;
    meow::db::ConnectionsManager _dbConnectionsManager;

    meow::settings::Core _settingsCore;

    Actions _actions;

    Log _log;
};

App * app();

} // namespace meow

#endif // MEOW_APP_H
