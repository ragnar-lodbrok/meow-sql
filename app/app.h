#ifndef MEOW_APP_H
#define MEOW_APP_H

#include "db/connection_params_manager.h"
#include "db/connections_manager.h"

#include "settings/settings_core.h"

#include "actions.h"

namespace meow {

class App
{
public:
    App();
    db::ConnectionParamsManager * dbConnectionParamsManager();
    db::ConnectionsManager * dbConnectionsManager();

    meow::settings::Core * settings() { return &_settingsCore; }

    Actions * actions() { return &_actions; }

private:
    meow::db::ConnectionParamsManager _dbConnectionParamsManager;
    meow::db::ConnectionsManager _dbConnectionsManager;

    meow::settings::Core _settingsCore;

    Actions _actions;
};

App * app();

} // namespace meow

#endif // MEOW_APP_H
