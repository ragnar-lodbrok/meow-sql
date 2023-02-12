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
    ~App();
    db::ConnectionParamsManager * dbConnectionParamsManager();
    db::ConnectionsManager * dbConnectionsManager();

    meow::settings::Core * settings() { return &_settingsCore; }

    Actions * actions() {
        if (!_actions) {
            _actions = new Actions(this);
        }
        return _actions;
    }

    Log * log() { return &_log; }

private:

    Log _log;

    meow::db::ConnectionParamsManager _dbConnectionParamsManager;
    std::shared_ptr<meow::db::ConnectionsManager> _dbConnectionsManager;

    meow::settings::Core _settingsCore;

    Actions * _actions = nullptr;
};

App * app();

} // namespace meow

#endif // MEOW_APP_H
